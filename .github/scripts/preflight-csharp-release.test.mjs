#!/usr/bin/env node

import assert from "node:assert/strict";
import test from "node:test";
import { evaluateCredentials } from "./preflight-csharp-release.mjs";

test("fails when either release credential is absent", () => {
  assert.deepEqual(evaluateCredentials({ githubToken: "", nugetToken: "" }), {
    passed: false,
    failures: ["GITHUB_TOKEN is unavailable", "NUGET_TOKEN is not configured"],
  });
});

test("passes when both release credentials are configured", () => {
  assert.deepEqual(
    evaluateCredentials({ githubToken: "github", nugetToken: "nuget" }),
    { passed: true, failures: [] },
  );
});
