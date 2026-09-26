#!/usr/bin/env node

/**
 * Tests for check-readme-badges.mjs.
 *
 * Run with: node --test .github/scripts/check-readme-badges.test.mjs
 */

import test from "node:test";
import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { mkdtempSync, mkdirSync, writeFileSync, rmSync } from "node:fs";
import { join, dirname, resolve } from "node:path";
import { tmpdir } from "node:os";
import { fileURLToPath } from "node:url";

const scriptPath = join(dirname(fileURLToPath(import.meta.url)), "check-readme-badges.mjs");
const repositoryRoot = resolve(dirname(fileURLToPath(import.meta.url)), "..", "..");

/** Workflows mirroring the real ones, so the fixtures stay realistic. */
const workflowFixtures = {
  "cpp-test.yml": "name: Test cpp\n",
  "csharp.yml": "name: csharp\n",
  "deploy-cpp.yml": "name: Deploy new cpp version\n",
  "quoted.yml": 'name: "Quoted name"\n',
};

/** Runs the check against a throwaway README and workflow directory. */
const runCheck = (readmeContents) => {
  const directory = mkdtempSync(join(tmpdir(), "readme-badges-"));
  try {
    const workflowsDirectory = join(directory, "workflows");
    mkdirSync(workflowsDirectory);
    for (const [fileName, contents] of Object.entries(workflowFixtures)) {
      writeFileSync(join(workflowsDirectory, fileName), contents);
    }
    const readmePath = join(directory, "README.md");
    writeFileSync(readmePath, readmeContents);

    const result = spawnSync(
      process.execPath,
      [scriptPath, "--readme", readmePath, "--workflows", workflowsDirectory, "--verbose"],
      { encoding: "utf8" },
    );
    return { status: result.status, output: `${result.stdout}${result.stderr}` };
  } finally {
    rmSync(directory, { recursive: true, force: true });
  }
};

const badge = (path) => `[![Actions Status](https://github.com/linksplatform/Interfaces/${path})](https://github.com/linksplatform/Interfaces/actions)`;

test("rejects the badge that broke in issue #141", () => {
  const { status, output } = runCheck(badge("workflows/deploy-cpp/badge.svg"));
  assert.equal(status, 1);
  assert.match(output, /workflow\(s\) that do not exist/);
  assert.match(output, /"deploy-cpp"/);
});

test("accepts a badge naming an existing workflow", () => {
  const { status, output } = runCheck(badge("workflows/Test%20cpp/badge.svg"));
  assert.equal(status, 0, output);
  assert.match(output, /ok badge \(workflow name\) by name: "Test cpp"/);
});

test("accepts the actions/workflows badge form without truncating the file name", () => {
  const { status, output } = runCheck(badge("actions/workflows/cpp-test.yml/badge.svg"));
  assert.equal(status, 0, output);
  assert.match(output, /ok badge \(workflow file\) by file: "cpp-test\.yml"/);
  assert.doesNotMatch(output, /cpp-test\.ym"/);
});

test("accepts a plain actions/workflows link", () => {
  const { status, output } = runCheck(`[link](https://github.com/linksplatform/Interfaces/actions/workflows/csharp.yml)`);
  assert.equal(status, 0, output);
  assert.match(output, /ok link \(workflow file\) by file: "csharp\.yml"/);
});

test("rejects a link to a workflow file that does not exist", () => {
  const { status, output } = runCheck(`[link](https://github.com/linksplatform/Interfaces/actions/workflows/missing.yml)`);
  assert.equal(status, 1);
  assert.match(output, /"missing\.yml"/);
});

test("decodes + as a space in actions?workflow= links", () => {
  const { status, output } = runCheck(`[link](https://github.com/linksplatform/Interfaces/actions?workflow=Deploy+new+cpp+version)`);
  assert.equal(status, 0, output);
  assert.match(output, /ok link \(workflow name\) by name: "Deploy new cpp version"/);
});

test("decodes %20 as a space in actions?workflow= links", () => {
  const { status, output } = runCheck(`[link](https://github.com/linksplatform/Interfaces/actions?workflow=Test%20cpp)`);
  assert.equal(status, 0, output);
  assert.match(output, /ok link \(workflow name\) by name: "Test cpp"/);
});

test("reads a quoted workflow name", () => {
  const { status, output } = runCheck(badge("workflows/Quoted%20name/badge.svg"));
  assert.equal(status, 0, output);
  assert.match(output, /ok badge \(workflow name\) by name: "Quoted name"/);
});

test("fails when README has no workflow badges at all", () => {
  const { status, output } = runCheck("# Interfaces\n\nNo badges here.\n");
  assert.equal(status, 1);
  assert.match(output, /No GitHub Actions badges found/);
});

test("the real README of this repository passes the check", () => {
  const result = spawnSync(process.execPath, [scriptPath], { cwd: repositoryRoot, encoding: "utf8" });
  assert.equal(result.status, 0, `${result.stdout}${result.stderr}`);
});
