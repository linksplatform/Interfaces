#!/usr/bin/env node

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import test from "node:test";

const workflowPath = new URL("../workflows/codeql.yml", import.meta.url);
const workflow = readFileSync(workflowPath, "utf8");

test("uses current CodeQL language identifiers and an explicit manual build", () => {
  assert.match(workflow, /language: \[ 'c-cpp', 'csharp' \]/);
  assert.match(workflow, /build-mode: manual/);
  assert.doesNotMatch(workflow, /language: \[ 'cpp'/);
});

test("installs every dependency required by the C++ CMake build", () => {
  assert.match(workflow, /sudo apt-get install -y[^\n]*libgtest-dev/);
  assert.match(workflow, /if: matrix\.language == 'c-cpp'/);
  assert.match(workflow, /cmake --build cpp\/build --parallel/);
});

test("uses action versions backed by the current Node runtime", () => {
  assert.match(workflow, /actions\/checkout@v7/);
  assert.match(workflow, /actions\/setup-dotnet@v6/);
  assert.match(workflow, /github\/codeql-action\/init@v4/);
  assert.match(workflow, /github\/codeql-action\/analyze@v4/);
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
