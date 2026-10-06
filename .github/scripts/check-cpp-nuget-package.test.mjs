#!/usr/bin/env node

import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { copyFileSync, mkdtempSync, readdirSync, rmSync } from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";
import { after, before, test } from "node:test";

// Packs the real package once, then checks it and broken copies of it with
// the real dotnet client and compiler, so this needs the .NET SDK, zip and c++.
const script = new URL("./check-cpp-nuget-package.sh", import.meta.url).pathname;
const packScript = new URL("./pack-cpp-nuget.mjs", import.meta.url).pathname;
const missing = ["dotnet", "zip", "c++"].filter(
  (tool) => spawnSync("bash", ["-c", `command -v ${tool}`]).status !== 0,
);
const options = { skip: missing.length > 0 && `${missing.join(", ")} not installed`, timeout: 300_000 };

let directory;
let packed;
before(() => {
  if (missing.length > 0) {
    return;
  }
  directory = mkdtempSync(path.join(tmpdir(), "check-cpp-nuget-"));
  const pack = spawnSync("node", [packScript, path.join(directory, "packed")], { encoding: "utf8" });
  assert.equal(pack.status, 0, pack.stdout + pack.stderr);
  const name = readdirSync(path.join(directory, "packed")).find((file) => file.endsWith(".nupkg"));
  packed = path.join(directory, "packed", name);
});
after(() => directory && rmSync(directory, { recursive: true, force: true }));

// Copies the package into its own folder, deletes `entries` from it and checks it.
const checkWithout = (label, entries) => {
  const copy = path.join(directory, label, path.basename(packed));
  spawnSync("mkdir", ["-p", path.dirname(copy)]);
  copyFileSync(packed, copy);
  if (entries.length > 0) {
    const zip = spawnSync("zip", ["-q", "-d", copy, ...entries], { encoding: "utf8" });
    assert.equal(zip.status, 0, zip.stderr);
  }
  return spawnSync("bash", [script, copy], { encoding: "utf8" });
};

test("restores and compiles against the packed package", options, () => {
  const result = checkWithout("complete", []);
  assert.equal(result.status, 0, result.stdout + result.stderr);
  assert.match(result.stdout, /NuGet restore and use of Platform\.Interfaces\.TemplateLibrary .*: OK/);
});

test("fails when the MSBuild targets are missing", options, () => {
  const result = checkWithout("no-targets", ["build/native/*"]);
  assert.notEqual(result.status, 0);
  assert.match(result.stdout, /::error title=C\+\+ NuGet package unusable::build\/native\/.*\.targets is missing/);
});

test("fails when a header that Platform.Interfaces.h includes is missing", options, () => {
  const result = checkWithout("no-header", ["lib/native/include/CArray.h"]);
  assert.notEqual(result.status, 0);
  assert.match(result.stdout, /does not compile/);
});
