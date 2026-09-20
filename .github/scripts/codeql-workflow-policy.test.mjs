#!/usr/bin/env node

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import test from "node:test";

const workflowPath = new URL("../workflows/codeql.yml", import.meta.url);
const workflow = readFileSync(workflowPath, "utf8");

test("uses current CodeQL language identifiers and build-free extraction", () => {
  assert.match(workflow, /language: \[ 'c-cpp', 'csharp' \]/);
  assert.match(workflow, /build-mode: none/);
  assert.doesNotMatch(workflow, /language: \[ 'cpp'/);
  assert.doesNotMatch(workflow, /build-mode: manual/);
});

test("does not duplicate the repositories' language build pipelines", () => {
  assert.doesNotMatch(workflow, /apt-get/);
  assert.doesNotMatch(workflow, /dotnet (?:restore|build)/);
  assert.doesNotMatch(workflow, /cmake/);
});

test("uses action versions backed by the current Node runtime", () => {
  assert.match(workflow, /actions\/checkout@v7/);
  assert.match(workflow, /github\/codeql-action\/init@v4/);
  assert.match(workflow, /github\/codeql-action\/analyze@v4/);
  assert.doesNotMatch(workflow, /actions\/setup-dotnet@/);
});

test("pins the runner and disables persisted checkout credentials", () => {
  assert.match(workflow, /runs-on: ubuntu-24\.04/);
  assert.doesNotMatch(workflow, /runs-on: ubuntu-latest/);
  assert.match(workflow, /persist-credentials: false/);
});

test("prevents checkout's default-branch warning", () => {
  assert.match(workflow, /GIT_CONFIG_COUNT: '1'/);
  assert.match(workflow, /GIT_CONFIG_KEY_0: init\.defaultBranch/);
  assert.match(workflow, /GIT_CONFIG_VALUE_0: main/);
});
