#!/usr/bin/env node

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import test from "node:test";
import { getJobs } from "./workflow-jobs.mjs";

const read = (name) => readFileSync(new URL(`../workflows/${name}`, import.meta.url), "utf8");
const testWorkflow = read("cpp-test.yml");
const deployWorkflow = read("deploy-cpp.yml");
const testJobs = getJobs(testWorkflow);
const deployJobs = getJobs(deployWorkflow);

test("keeps the workflow names that README badges reference", () => {
  assert.match(testWorkflow, /^name: Test cpp$/m);
  assert.match(deployWorkflow, /^name: Deploy new cpp version$/m);
});

test("tests pull requests whenever C++ sources change", () => {
  const pullRequest = testWorkflow.slice(
    testWorkflow.indexOf("  pull_request:"),
    testWorkflow.indexOf("  workflow_call:"),
  );
  assert.doesNotMatch(pullRequest, /types:/);
  assert.match(pullRequest, /- 'cpp\/\*\*'/);
});

test("runs the tests instead of only compiling them", () => {
  const job = testJobs.get("test");
  assert.match(job, /run: cpp\/build-and-test\.sh/);
  const script = readFileSync(new URL("../../cpp/build-and-test.sh", import.meta.url), "utf8");
  assert.match(script, /ctest .*--no-tests=error/);
});

test("packs with dotnet and verifies the package on every run", () => {
  const job = testJobs.get("pack");
  assert.match(job, /pack-cpp-nuget\.mjs "\$RUNNER_TEMP\/cpp-nuget"/);
  assert.match(job, /name: cpp-nuget-package/);
  assert.match(job, /if-no-files-found: error/);
});

test("deploys only what the local test workflow built and tested", () => {
  assert.match(deployJobs.get("test"), /uses: \.\/\.github\/workflows\/cpp-test\.yml/);
  assert.match(deployJobs.get("releasePreflight"), /needs: test/);
  assert.match(deployJobs.get("releasePreflight"), /preflight-nuget-release\.mjs/);
  assert.match(deployJobs.get("pushToNuget"), /needs: \[test, releasePreflight\]/);
  assert.match(deployJobs.get("pushToNuget"), /push-nuget-package\.sh "\$RUNNER_TEMP\/cpp-nuget"\/\*\.nupkg/);
});

test("supports trusted publishing with the API key as a fallback", () => {
  const job = deployJobs.get("pushToNuget");
  assert.match(job, /\n      id-token: write/);
  assert.match(job, /uses: NuGet\/login@[0-9a-f]{40} # v\d/);
  assert.match(job, /NUGET_API_KEY: \$\{\{ steps\.nuget-login\.outputs\.NUGET_API_KEY \|\| secrets\.NUGET_TOKEN \}\}/);
});

test("creates the GitHub release only after the NuGet push succeeded", () => {
  const job = deployJobs.get("publishRelease");
  assert.match(job, /needs: \[test, pushToNuget\]/);
  assert.doesNotMatch(job, /\n    if:/);
  assert.match(job, /gh release create "\$tag" "\$archive"/);
});
