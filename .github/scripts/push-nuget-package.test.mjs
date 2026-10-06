#!/usr/bin/env node

import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { chmodSync, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";
import test from "node:test";

const script = new URL("./push-nuget-package.sh", import.meta.url).pathname;
// The exact line nuget.org returned in run 36213154898 (issue #150).
const rejectedKey =
  "error: Response status code does not indicate success: 403 (The specified API key is invalid, has expired, or does not have permission to access the specified package.).";

const withFakeDotnet = (dotnetBody, callback) => {
  const directory = mkdtempSync(path.join(tmpdir(), "push-nuget-"));
  try {
    const dotnet = path.join(directory, "dotnet");
    writeFileSync(dotnet, `#!/usr/bin/env bash\necho "$@" > "${directory}/args"\n${dotnetBody}\n`);
    chmodSync(dotnet, 0o755);
    const nupkg = path.join(directory, "P.1.0.0.nupkg");
    writeFileSync(nupkg, "package");
    return callback({ directory, nupkg });
  } finally {
    rmSync(directory, { recursive: true, force: true });
  }
};

const push = (directory, nupkg, environment) =>
  spawnSync("bash", [script, nupkg], {
    encoding: "utf8",
    env: { PATH: `${directory}:${process.env.PATH}`, ...environment },
  });

test("skips the push when the version is already published", () => {
  withFakeDotnet("exit 99", ({ directory, nupkg }) => {
    const result = push(directory, nupkg, { PUBLISH_REQUIRED: "false" });
    assert.equal(result.status, 0);
    assert.match(result.stdout, /already on nuget\.org/);
  });
});

test("fails without a key instead of silently skipping", () => {
  withFakeDotnet("exit 0", ({ directory, nupkg }) => {
    const result = push(directory, nupkg, { PUBLISH_REQUIRED: "true" });
    assert.equal(result.status, 1);
    assert.match(result.stdout, /::error title=NuGet push failed::No API key/);
  });
});

test("pushes with --skip-duplicate to nuget.org", () => {
  withFakeDotnet("exit 0", ({ directory, nupkg }) => {
    const result = push(directory, nupkg, { NUGET_API_KEY: "key" });
    assert.equal(result.status, 0);
    const args = spawnSync("cat", [path.join(directory, "args")], { encoding: "utf8" }).stdout;
    assert.match(args, /^nuget push .*P\.1\.0\.0\.nupkg --source https:\/\/api\.nuget\.org\/v3\/index\.json --api-key key --skip-duplicate/);
  });
});

test("explains how to renew an expired NUGET_TOKEN", () => {
  withFakeDotnet(`echo '${rejectedKey}'; exit 1`, ({ directory, nupkg }) => {
    const result = push(directory, nupkg, { NUGET_API_KEY: "expired", AUTH: "api-key" });
    assert.equal(result.status, 1);
    assert.match(result.stdout, /::error title=nuget\.org rejected NUGET_TOKEN::/);
    assert.match(result.stdout, /trusted-publishing/);
  });
});

test("explains how to fix a rejected trusted publishing policy", () => {
  withFakeDotnet(`echo '${rejectedKey}'; exit 1`, ({ directory, nupkg }) => {
    const result = push(directory, nupkg, {
      NUGET_API_KEY: "short-lived",
      AUTH: "trusted-publishing",
      GITHUB_REPOSITORY: "linksplatform/Interfaces",
      GITHUB_WORKFLOW_REF: "linksplatform/Interfaces/.github/workflows/csharp.yml@refs/heads/main",
    });
    assert.equal(result.status, 1);
    assert.match(result.stdout, /rejected the trusted publishing key::.*linksplatform\/Interfaces .*csharp\.yml/);
  });
});

test("keeps other push failures red without misleading guidance", () => {
  withFakeDotnet("echo 'error: 500 (Internal Server Error)'; exit 1", ({ directory, nupkg }) => {
    const result = push(directory, nupkg, { NUGET_API_KEY: "key" });
    assert.equal(result.status, 1);
    assert.doesNotMatch(result.stdout, /rejected/);
  });
});
