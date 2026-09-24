#!/usr/bin/env node

import assert from "node:assert/strict";
import { mkdtempSync, mkdirSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { spawnSync } from "node:child_process";
import test from "node:test";

const validator = new URL("./validate-csharp-docs.sh", import.meta.url).pathname;

const withSite = (files, check) => {
  const site = mkdtempSync(join(tmpdir(), "csharp-docs-"));
  try {
    for (const file of files) {
      const path = join(site, file);
      mkdirSync(join(path, ".."), { recursive: true });
      writeFileSync(path, "generated content");
    }
    check(spawnSync("bash", [validator, site], { encoding: "utf8" }));
  } finally {
    rmSync(site, { recursive: true, force: true });
  }
};

const requiredFiles = [
  "index.html",
  "api/Platform.Interfaces.html",
  "api/Platform.Interfaces.IFactory-1.html",
  "xrefmap.yml",
];

test("accepts a complete DocFX site", () => {
  withSite(requiredFiles, ({ status, stderr }) => {
    assert.equal(status, 0, stderr);
  });
});

for (const missingFile of requiredFiles) {
  test(`rejects a site missing ${missingFile}`, () => {
    withSite(requiredFiles.filter((file) => file !== missingFile), ({ status, stderr }) => {
      assert.notEqual(status, 0);
      assert.ok(stderr.includes(missingFile), stderr);
    });
  });
}
