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
  assert.match(job, /\n {10}cpp\/build-and-test\.sh\n/);
  const script = readFileSync(new URL("../../cpp/build-and-test.sh", import.meta.url), "utf8");
  assert.match(script, /ctest .*--no-tests=error/);
});

test("tests every compiler that consumes the headers, with sanitizers", () => {
  const job = testJobs.get("test");
  for (const os of ["ubuntu-24.04", "macos-15", "windows-2025"]) {
    assert.match(job, new RegExp(`os: ${os}\\n`));
  }
  assert.match(job, /cxx: g\+\+/);
  assert.match(job, /cxx: clang\+\+/);
  assert.match(job, /sanitizers: address;undefined/);
  assert.match(job, /fail-fast: false/);
  // Matrix jobs share a job-level group unless it names the matrix entry.
  assert.match(job, /group: .*\$\{\{ matrix\.name \}\}/);
  const cmake = readFileSync(new URL("../../cpp/CMakeLists.txt", import.meta.url), "utf8");
  assert.match(cmake, /-fsanitize=\$\{sanitizers\}/);
});

test("lets the build script decide whether tests are built", () => {
  const cmake = readFileSync(new URL("../../cpp/CMakeLists.txt", import.meta.url), "utf8");
  assert.doesNotMatch(cmake, /set\(LINKS_PLATFORM_TESTS (TRUE|ON)\)/);
  assert.doesNotMatch(cmake, /CMAKE_CXX_FLAGS.*-march/);
  const script = readFileSync(new URL("../../cpp/build-and-test.sh", import.meta.url), "utf8");
  assert.match(script, /-DLINKS_PLATFORM_TESTS=ON/);
  assert.match(script, /cmake --build .*--config "\$build_type"/);
  assert.match(script, /ctest .*--build-config "\$build_type"/);
});

test("checks the C++ formatting with a pinned clang-format", () => {
  const job = testJobs.get("format");
  assert.match(job, /git submodule update --init --depth 1 Settings/);
  assert.match(job, /pipx install clang-format==\d+\.\d+\.\d+\n/);
  assert.match(job, /run: \.github\/scripts\/check-cpp-format\.sh\n/);
  const script = readFileSync(new URL("./check-cpp-format.sh", import.meta.url), "utf8");
  assert.match(script, /--dry-run --Werror/);
});

test("packs with dotnet and verifies the package on every run", () => {
  const job = testJobs.get("pack");
  assert.match(job, /pack-cpp-nuget\.mjs "\$RUNNER_TEMP\/cpp-nuget"/);
  assert.match(job, /check-cpp-nuget-package\.sh "\$PACKAGE_PATH"/);
  assert.match(job, /check-cpp-nuget-package\.test\.mjs/);
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
