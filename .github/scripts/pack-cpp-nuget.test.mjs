#!/usr/bin/env node

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import test from "node:test";
import {
  expectedEntries,
  findMissingEntries,
  nuspecPath,
  readPackageInfo,
  withStagedFiles,
} from "./pack-cpp-nuget.mjs";

const nuspec = readFileSync(nuspecPath, "utf8");

test("reads the package identity and release notes from the nuspec", () => {
  const info = readPackageInfo(nuspec);
  assert.equal(info.id, "Platform.Interfaces.TemplateLibrary");
  assert.match(info.version, /^\d+\.\d+\.\d+$/);
  assert.ok(info.releaseNotes.length > 0);
});

test("fails loudly when a required nuspec element is missing", () => {
  assert.throws(
    () => readPackageInfo("<package><metadata><id>x</id></metadata></package>"),
    /no <version> element/,
  );
});

test("adds the staged content after the metadata without editing it", () => {
  const staged = withStagedFiles(nuspec);
  assert.ok(staged.startsWith(nuspec.slice(0, nuspec.indexOf("</metadata>"))));
  assert.match(staged, /<\/metadata>\n  <files>\n    <file src="content\/\*\*" target="" \/>\n  <\/files>/);
  assert.throws(() => withStagedFiles(staged), /already lists <files>/);
});

test("expects the layout of the last package published to nuget.org", () => {
  assert.deepEqual(expectedEntries("Id", ["A.h", "B[T].h"]), [
    "images/icon.png",
    "build/native/Id.targets",
    "lib/native/include/A.h",
    "lib/native/include/B[T].h",
  ]);
});

test("reports every expected entry that the package lacks", () => {
  assert.deepEqual(
    findMissingEntries(["images/icon.png", "lib/native/include/A.h"], ["images/icon.png"]),
    ["lib/native/include/A.h"],
  );
});
