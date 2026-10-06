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
  verifyApiKey,
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
  assert.equal(result.warnings.length, 2);
  assert.match(result.warnings[1], /could not verify the NUGET_TOKEN/);
});

test("says why the API key could not be verified", () => {
  const result = evaluateRelease({
    published: false,
    githubToken: "g",
    nugetToken: "k",
    nugetUser: "",
    keyValid: undefined,
    keyDetail: "no published version exists to check its package scope",
  });
  assert.equal(result.passed, true);
  assert.match(result.warnings[0], /NUGET_TOKEN secret \(no published version exists .*\); the push/);
});

// Answers each request from a "METHOD url" -> [status, body] table and records the calls.
const gallery = (routes) => {
  const calls = [];
  const fetchImpl = async (url, options = {}) => {
    const request = `${options.method ?? "GET"} ${url}`;
    calls.push({ request, key: options.headers?.["X-NuGet-ApiKey"] });
    const route = routes[request];
    if (!route) {
      throw new Error(`unexpected ${request}`);
    }
    return respond(...route)();
  };
  return { calls, fetchImpl };
};
const createUrl = "POST https://www.nuget.org/api/v2/package/create-verification-key/Platform.Interfaces";
const verifyUrl = "GET https://www.nuget.org/api/v2/verifykey/Platform.Interfaces/0.5.0";

test("verifies an API key in two steps without publishing", async () => {
  const { calls, fetchImpl } = gallery({
    [createUrl]: [200, { Key: "one-time", Expires: "2026-10-07T00:00:00Z" }],
    [verifyUrl]: [200],
  });
  const result = await verifyApiKey({
    packageId: "Platform.Interfaces",
    publishedVersion: "0.5.0",
    apiKey: "secret",
    fetchImpl,
  });
  assert.equal(result.valid, true);
  assert.deepEqual(calls, [
    { request: createUrl, key: "secret" },
    { request: verifyUrl, key: "one-time" },
  ]);
});

test("rejects an invalid or expired API key (issue #150)", async () => {
  for (const status of [401, 403]) {
    const { calls, fetchImpl } = gallery({ [createUrl]: [status] });
    const result = await verifyApiKey({
      packageId: "Platform.Interfaces",
      publishedVersion: "0.5.0",
      apiKey: "expired",
      fetchImpl,
    });
    assert.equal(result.valid, false);
    assert.equal(calls.length, 1);
  }
});

test("rejects an API key that is not scoped to the package", async () => {
  const { fetchImpl } = gallery({
    [createUrl]: [200, { Key: "one-time" }],
    [verifyUrl]: [403],
  });
  const result = await verifyApiKey({
    packageId: "Platform.Interfaces",
    publishedVersion: "0.5.0",
    apiKey: "other-glob",
    fetchImpl,
  });
  assert.equal(result.valid, false);
});

test("does not claim a package scope it cannot check for a package that was never published", async () => {
  const { calls, fetchImpl } = gallery({ [createUrl]: [200, { Key: "one-time" }] });
  const result = await verifyApiKey({ packageId: "Platform.Interfaces", apiKey: "k", fetchImpl });
  assert.equal(result.valid, undefined);
  assert.match(result.detail, /no published version/);
  assert.equal(calls.length, 1);
});

test("reports an unknown key state when nuget.org cannot answer", async () => {
  const verify = (fetchImpl) =>
    verifyApiKey({ packageId: "Platform.Interfaces", publishedVersion: "0.5.0", apiKey: "k", fetchImpl });
  assert.equal((await verify(gallery({ [createUrl]: [503] }).fetchImpl)).valid, undefined);
  assert.equal((await verify(gallery({ [createUrl]: [200, {}] }).fetchImpl)).valid, undefined);
  assert.equal(
    (await verify(gallery({ [createUrl]: [200, { Key: "one-time" }], [verifyUrl]: [500] }).fetchImpl)).valid,
    undefined,
  );
  assert.equal((await verify(gallery({}).fetchImpl)).valid, undefined);
});

test("fails before pushing when nuget.org rejects NUGET_TOKEN", () => {
  const result = evaluateRelease({
    published: false,
    githubToken: "g",
    nugetToken: "expired",
    nugetUser: "",
    keyValid: false,
  });
  assert.equal(result.passed, false);
  assert.match(result.failures[0], /rejected the NUGET_TOKEN secret/);
});

test("does not verify NUGET_TOKEN when nothing has to be published", async () => {
  const { calls, fetchImpl } = gallery({
    "GET https://api.nuget.org/v3-flatcontainer/platform.interfaces/index.json": [200, { versions: ["0.5.0"] }],
  });
  const exitCode = await run(
    { PACKAGE_ID: "Platform.Interfaces", PACKAGE_VERSION: "0.5.0", GITHUB_TOKEN: "g", NUGET_TOKEN: "expired" },
    fetchImpl,
  );
  assert.equal(exitCode, 0);
  assert.equal(calls.length, 1);
});

test("fails the preflight with an expired NUGET_TOKEN for an unpublished version", async () => {
  const { calls, fetchImpl } = gallery({
    "GET https://api.nuget.org/v3-flatcontainer/platform.interfaces/index.json": [200, { versions: ["0.4.0", "0.5.0"] }],
    [createUrl]: [403],
  });
  const exitCode = await run(
    { PACKAGE_ID: "Platform.Interfaces", PACKAGE_VERSION: "0.6.1", GITHUB_TOKEN: "g", NUGET_TOKEN: "expired" },
    fetchImpl,
  );
  assert.equal(exitCode, 1);
  assert.equal(calls.length, 2);
});

test("verifies the package scope against the newest published version", async () => {
  const { calls, fetchImpl } = gallery({
    "GET https://api.nuget.org/v3-flatcontainer/platform.interfaces/index.json": [200, { versions: ["0.4.0", "0.5.0"] }],
    [createUrl]: [200, { Key: "one-time" }],
    [verifyUrl]: [200],
  });
  const exitCode = await run(
    { PACKAGE_ID: "Platform.Interfaces", PACKAGE_VERSION: "0.6.1", GITHUB_TOKEN: "g", NUGET_TOKEN: "valid" },
    fetchImpl,
  );
  assert.equal(exitCode, 0);
  assert.equal(calls.at(-1).request, verifyUrl);
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
