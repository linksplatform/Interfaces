#!/usr/bin/env node

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import test from "node:test";

const workflowPath = new URL("../workflows/csharp.yml", import.meta.url);
const workflow = readFileSync(workflowPath, "utf8");

const getJobs = (source) => {
  const jobs = new Map();
  const lines = source.slice(source.indexOf("\njobs:\n") + 1).split("\n");
  let currentName;
  let currentLines = [];

  for (const line of lines.slice(1)) {
    const match = /^  ([A-Za-z][A-Za-z0-9]*):\s*$/.exec(line);
    if (match) {
      if (currentName) {
        jobs.set(currentName, currentLines.join("\n"));
      }
      currentName = match[1];
      currentLines = [line];
    } else if (currentName) {
      currentLines.push(line);
    }
  }
  if (currentName) {
    jobs.set(currentName, currentLines.join("\n"));
  }
  return jobs;
};

const jobs = getJobs(workflow);

test("configures Git's initial branch before checkout runs", () => {
  const topLevel = workflow.slice(0, workflow.indexOf("\ndefaults:"));
  assert.match(topLevel, /\n  GIT_CONFIG_COUNT: '1'/);
  assert.match(topLevel, /\n  GIT_CONFIG_KEY_0: init\.defaultBranch/);
  assert.match(topLevel, /\n  GIT_CONFIG_VALUE_0: main/);
});

test("validates every maintained C# workflow input on pull requests", () => {
  assert.match(workflow, /pull_request:/);
  assert.match(workflow, /- '\.github\/scripts\/\*csharp\*'/);
  assert.match(workflow, /- 'csharp\/\*\*'/);
});

test("uses repository-owned build scripts instead of mutable downloads", () => {
  assert.doesNotMatch(workflow, /raw\.githubusercontent\.com/);
  assert.doesNotMatch(workflow, /\bwget\b/);
  assert.doesNotMatch(workflow, /python-pygments/);
  assert.doesNotMatch(workflow, /apt-get install(?:\s+-y)?\s+nuget/);
});

test("declares least-privilege checkout credentials", () => {
  const lines = workflow.split("\n");
  const failures = [];

  for (let index = 0; index < lines.length; index += 1) {
    if (!lines[index].includes("uses: actions/checkout@")) {
      continue;
    }
    const step = lines.slice(index, index + 8).join("\n");
    if (!/persist-credentials:\s*false/.test(step)) {
      failures.push(`line ${index + 1}: ${lines[index].trim()}`);
    }
  }

  assert.deepEqual(failures, []);
});

test("caps every job so hangs cannot run indefinitely", () => {
  assert.ok(jobs.size > 0, "workflow jobs should be parsed");
  for (const [name, job] of jobs) {
    assert.match(job, /\n    timeout-minutes:\s*\d+/, `${name} has no timeout`);
  }
});

test("pins the runner image so latest-image migrations do not create warnings", () => {
  assert.doesNotMatch(workflow, /runs-on: ubuntu-latest/);
  for (const [name, job] of jobs) {
    assert.match(job, /\n    runs-on: ubuntu-24\.04/, `${name} has an unpinned runner`);
  }
});

test("gates both package publishers on a release preflight", () => {
  const preflight = jobs.get("releasePreflight");
  assert.ok(preflight, "releasePreflight job should exist");
  assert.match(preflight, /NUGET_TOKEN: \$\{\{ secrets\.NUGET_TOKEN \}\}/);
  assert.match(preflight, /GITHUB_TOKEN: \$\{\{ secrets\.GITHUB_TOKEN \}\}/);
  assert.match(preflight, /preflight-csharp-release\.mjs/);

  for (const name of [
    "pushNuGetPackageToGitHubPackageRegistry",
    "pushToNuget",
  ]) {
    assert.match(
      jobs.get(name),
      /needs: \[test, releasePreflight\]/,
      `${name} must wait for validation and preflight`,
    );
  }
});

test("builds PDF and API documentation in parallel before publishing both", () => {
  const pdf = jobs.get("generatePdfWithCode");
  const documentation = jobs.get("buildDocumentation");
  const publisher = jobs.get("publishDocumentation");

  assert.ok(pdf, "generatePdfWithCode job should exist");
  assert.ok(documentation, "buildDocumentation job should exist");
  assert.ok(publisher, "publishDocumentation job should exist");
  assert.match(pdf, /needs: \[findChangedCsFiles\]/);
  assert.match(documentation, /needs: \[findChangedCsFiles\]/);
  assert.doesNotMatch(documentation, /needs:.*generatePdfWithCode/);
  assert.doesNotMatch(pdf, /github\.event_name == 'push'/);
  assert.doesNotMatch(documentation, /github\.event_name == 'push'/);
  assert.match(pdf, /name: csharp-pdf/);
  assert.match(documentation, /name: csharp-documentation/);
  assert.doesNotMatch(workflow, /actions\/download-artifact@/);
  assert.match(
    publisher,
    /needs: \[findChangedCsFiles, generatePdfWithCode, buildDocumentation\]/,
  );
  assert.match(publisher, /--name csharp-documentation/);
  assert.match(publisher, /--name csharp-pdf/);
  assert.match(
    publisher,
    /if: \$\{\{ needs\.findChangedCsFiles\.outputs\.documentationChanged == 'true' \}\}/,
  );
  assert.match(
    publisher,
    /- name: Publish documentation to gh-pages\n        if: \$\{\{ github\.event_name == 'push' \}\}/,
  );
});

test("aggregates every job result so skipped dependents cannot hide failures", () => {
  const gate = jobs.get("pipelineStatus");
  assert.ok(gate, "pipelineStatus job should exist");
  assert.match(gate, /if: \$\{\{ always\(\) \}\}/);
  assert.match(gate, /check-csharp-pipeline-status\.mjs/);

  for (const name of [...jobs.keys()].filter((name) => name !== "pipelineStatus")) {
    assert.match(gate, new RegExp(`\\n      - ${name}(?:\\n|$)`), `${name} is not observed`);
  }
});
