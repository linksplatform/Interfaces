#!/usr/bin/env node

import assert from "node:assert/strict";
import { mkdtempSync, readFileSync, rmSync } from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";
import test from "node:test";
import {
  evaluateRelease,
  fetchPublished,
  readCsproj,
  run,
  versionIndexUrl,
} from "./preflight-nuget-release.mjs";

const respond = (status, body = {}) => async () => ({
  ok: status >= 200 && status < 300,
  status,
  json: async () => body,
});

test("reads the C# package identity from the project file", () => {
  const csproj = readFileSync(
    new URL("../../csharp/Platform.Interfaces/Platform.Interfaces.csproj", import.meta.url),
    "utf8",
  );
  const project = readCsproj(csproj);
  assert.equal(project.packageId, "Platform.Interfaces");
  assert.match(project.version, /^\d+\.\d+\.\d+$/);
});

test("queries the lower-case flat-container index", () => {
  assert.equal(
    versionIndexUrl("Platform.Interfaces.TemplateLibrary"),
    "https://api.nuget.org/v3-flatcontainer/platform.interfaces.templatelibrary/index.json",
  );
});

test("detects published, unpublished, unknown and unreachable packages", async () => {
  const query = (fetchImpl) =>
    fetchPublished({ packageId: "P", version: "1.0.0-Beta", fetchImpl });
  assert.equal((await query(respond(200, { versions: ["1.0.0-beta"] }))).published, true);
  assert.equal((await query(respond(200, { versions: ["0.9.0"] }))).published, false);
  assert.equal((await query(respond(404))).published, false);
  assert.equal((await query(respond(503))).published, undefined);
  assert.equal(
    (await query(async () => {
      throw new Error("offline");
    })).published,
    undefined,
  );
});

test("skips publishing an existing version without NuGet credentials", () => {
  assert.deepEqual(
    evaluateRelease({ published: true, githubToken: "g", nugetToken: "", nugetUser: "" }),
    { passed: true, publishRequired: false, auth: "none", failures: [], warnings: [] },
  );
});

test("fails before pushing when an unpublished version has no credentials", () => {
  const result = evaluateRelease({ published: false, githubToken: "", nugetToken: "", nugetUser: "" });
  assert.equal(result.passed, false);
  assert.equal(result.publishRequired, true);
  assert.deepEqual(result.failures.length, 2);
  assert.match(result.failures[1], /NUGET_USER .* NUGET_TOKEN/);
});

test("prefers trusted publishing over the long-lived API key", () => {
  const result = evaluateRelease({ published: false, githubToken: "g", nugetToken: "k", nugetUser: "linksplatform" });
  assert.equal(result.auth, "trusted-publishing");
  assert.equal(result.passed, true);
});

test("still publishes with --skip-duplicate when nuget.org is unreachable", () => {
  const result = evaluateRelease({ published: undefined, githubToken: "g", nugetToken: "k", nugetUser: "" });
  assert.equal(result.passed, true);
  assert.equal(result.publishRequired, true);
  assert.equal(result.auth, "api-key");
  assert.equal(result.warnings.length, 1);
});

test("writes the decision to GITHUB_OUTPUT", async () => {
  const directory = mkdtempSync(path.join(tmpdir(), "preflight-"));
  const output = path.join(directory, "output");
  try {
    const exitCode = await run(
      {
        PACKAGE_ID: "Platform.Interfaces",
        PACKAGE_VERSION: "0.5.0",
        GITHUB_TOKEN: "g",
        GITHUB_OUTPUT: output,
      },
      respond(200, { versions: ["0.5.0"] }),
    );
    assert.equal(exitCode, 0);
    assert.equal(readFileSync(output, "utf8"), "publish_required=false\nauth=none\n");
  } finally {
    rmSync(directory, { recursive: true, force: true });
  }
});
