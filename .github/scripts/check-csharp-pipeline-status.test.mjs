#!/usr/bin/env node

import assert from "node:assert/strict";
import test from "node:test";
import { evaluatePipeline } from "./check-csharp-pipeline-status.mjs";

const results = (overrides = {}) =>
  Object.fromEntries(
    [
      "test",
      "releasePreflight",
      "pushNuGetPackageToGitHubPackageRegistry",
      "pushToNuget",
      "publishRelease",
      "findChangedCsFiles",
      "generatePdfWithCode",
      "buildDocumentation",
      "publishDocumentation",
    ].map((name) => [name, { result: overrides[name] ?? "skipped" }]),
  );

test("accepts a pull request after validation and documentation builds pass", () => {
  const needs = results({
    test: "success",
    findChangedCsFiles: "success",
    generatePdfWithCode: "success",
    buildDocumentation: "success",
    publishDocumentation: "success",
  });
  assert.deepEqual(
    evaluatePipeline({ eventName: "pull_request", documentationChanged: true, needs }),
    { passed: true, failures: [] },
  );
});

test("rejects a skipped documentation assembly on pull requests", () => {
  const needs = results({
    test: "success",
    findChangedCsFiles: "success",
    generatePdfWithCode: "success",
    buildDocumentation: "success",
  });
  assert.deepEqual(
    evaluatePipeline({ eventName: "pull_request", documentationChanged: true, needs }),
    {
      passed: false,
      failures: ["publishDocumentation: required job finished with skipped"],
    },
  );
});

test("accepts expected documentation skips when no maintained input changed", () => {
  const needs = results({
    test: "success",
    releasePreflight: "success",
    pushNuGetPackageToGitHubPackageRegistry: "success",
    pushToNuget: "success",
    publishRelease: "success",
    findChangedCsFiles: "success",
  });
  assert.deepEqual(
    evaluatePipeline({ eventName: "push", documentationChanged: false, needs }),
    { passed: true, failures: [] },
  );
});

test("turns a failed publish and its skipped release into an explicit failure", () => {
  const needs = results({
    test: "success",
    releasePreflight: "success",
    pushNuGetPackageToGitHubPackageRegistry: "success",
    pushToNuget: "failure",
    publishRelease: "skipped",
    findChangedCsFiles: "success",
  });
  const result = evaluatePipeline({
    eventName: "push",
    documentationChanged: false,
    needs,
  });

  assert.equal(result.passed, false);
  assert.deepEqual(result.failures, [
    "pushToNuget: required job finished with failure",
    "publishRelease: required job finished with skipped",
  ]);
});

test("reports cancelled jobs instead of allowing a grey timeout", () => {
  const needs = results({
    test: "cancelled",
    findChangedCsFiles: "skipped",
  });
  const result = evaluatePipeline({
    eventName: "pull_request",
    documentationChanged: false,
    needs,
  });

  assert.equal(result.passed, false);
  assert.match(result.failures.join("\n"), /test: required job finished with cancelled/);
});
