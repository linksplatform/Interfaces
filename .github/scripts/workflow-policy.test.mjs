#!/usr/bin/env node

// Repository-wide guards for the CI/CD problems found in issue #150, so a
// fix applied to one workflow cannot regress in another.

import assert from "node:assert/strict";
import { readdirSync, readFileSync } from "node:fs";
import test from "node:test";
import { getJobs } from "./workflow-jobs.mjs";

const workflowsDirectory = new URL("../workflows/", import.meta.url);
const workflows = readdirSync(workflowsDirectory)
  .filter((name) => /\.ya?ml$/.test(name))
  .map((name) => ({ name, source: readFileSync(new URL(name, workflowsDirectory), "utf8") }));

const findLines = (pattern) =>
  workflows.flatMap(({ name, source }) =>
    source
      .split("\n")
      .map((line, index) => ({ line, location: `${name}:${index + 1}` }))
      .filter(({ line }) => !line.trimStart().startsWith("#") && pattern.test(line))
      .map(({ line, location }) => `${location}: ${line.trim()}`),
  );

test("finds every workflow", () => {
  assert.ok(workflows.length >= 8, `only ${workflows.length} workflows found`);
});

test("pins runner images so ubuntu-latest migrations cannot change builds", () => {
  assert.deepEqual(findLines(/runs-on:\s*ubuntu-latest/), []);
});

test("caps every job that runs on a runner", () => {
  const failures = [];
  for (const { name, source } of workflows) {
    for (const [job, body] of getJobs(source)) {
      if (/\n    runs-on:/.test(body) && !/\n    timeout-minutes:\s*\d+/.test(body)) {
        failures.push(`${name}: ${job}`);
      }
    }
  }
  assert.deepEqual(failures, []);
});

test("declares least-privilege token permissions in every workflow", () => {
  const failures = workflows
    .filter(({ source }) => !/^permissions:/m.test(source))
    .map(({ name }) => name);
  assert.deepEqual(failures, []);
});

test("never persists checkout credentials", () => {
  const failures = [];
  for (const { name, source } of workflows) {
    const lines = source.split("\n");
    lines.forEach((line, index) => {
      if (line.includes("uses: actions/checkout@")) {
        const step = lines.slice(index, index + 8).join("\n");
        if (!/persist-credentials:\s*false/.test(step)) {
          failures.push(`${name}:${index + 1}`);
        }
      }
    });
  }
  assert.deepEqual(failures, []);
});

test("uses no actions that run on the deprecated Node 20 runtime", () => {
  assert.deepEqual(
    findLines(
      /uses:\s*(?:actions\/(?:checkout|setup-dotnet|setup-node|setup-python|cache)@v[1-4]\b|actions\/(?:upload|download)-artifact@v[1-4]\b|ncipollo\/release-action|egor-tensin\/setup-gcc|ahmadnassri\/)/,
    ),
    [],
  );
});

test("pins third-party actions to a full commit hash", () => {
  const trusted = /^(?:actions|github)\//;
  const failures = findLines(/^\s*(?:-\s*)?uses:\s*[^.\s]/).filter((entry) => {
    const reference = /uses:\s*(\S+)/.exec(entry)[1];
    if (reference.startsWith("docker://")) {
      return !/@sha256:[0-9a-f]{64}$/.test(reference);
    }
    return !trusted.test(reference) && !/@[0-9a-f]{40}$/.test(reference);
  });
  assert.deepEqual(failures, []);
});

test("keeps CI definitions in this repository instead of a mutable @main", () => {
  assert.deepEqual(findLines(/uses:\s*linksplatform\/Workflows\//), []);
  assert.deepEqual(findLines(/uses:\s*[^#\s]+@(?:main|master)\b/), []);
});

test("does not install the nuget CLI that Ubuntu 24.04 no longer ships", () => {
  assert.deepEqual(findLines(/apt-get install[^#\n]*\bnuget\b/), []);
});

test("runs pull request checks on code changes, not only on edits", () => {
  assert.deepEqual(findLines(/^\s*-\s*edited\s*$|types:\s*\[\s*edited\s*\]/), []);
});

test("does not trust the spoofable github.actor for bot checks", () => {
  assert.deepEqual(findLines(/github\.(?:triggering_)?actor\s*==/), []);
});

test("lets cancelled runs stop instead of forcing jobs with always()", () => {
  assert.deepEqual(findLines(/\balways\(\)/), []);
});

test("lets every workflow supersede stale runs without cancelling a writer", () => {
  const missing = workflows
    .filter(({ source }) => !/^concurrency:/m.test(source))
    .filter(({ source }) =>
      [...getJobs(source).values()].some((body) => /\n    runs-on:/.test(body) && !/\n    concurrency:/.test(body)),
    )
    .map(({ name }) => name);
  assert.deepEqual(missing, []);

  const writers = workflows
    .filter(({ source }) => /^\s+(?:contents|packages|id-token|pages):\s*write/m.test(source))
    .filter(({ source }) => /cancel-in-progress:\s*true/.test(source))
    .map(({ name }) => name);
  assert.deepEqual(writers, []);
});

test("scans the repository for committed secrets with a pinned scanner", () => {
  const secrets = workflows.find(({ name }) => name === "secrets.yml");
  assert.ok(secrets, "secrets.yml is missing");
  assert.match(secrets.source, /secretlint@\d+\.\d+\.\d+/);
  assert.match(secrets.source, /@secretlint\/secretlint-rule-preset-recommend@\d+\.\d+\.\d+/);
  assert.doesNotMatch(secrets.source, /\n    paths:/);
});
