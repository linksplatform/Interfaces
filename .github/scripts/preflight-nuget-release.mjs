#!/usr/bin/env node

/**
 * Decide whether a NuGet release still has to be published and check the
 * credentials that publishing needs, before any package is pushed.
 *
 * Issue #150 found two blind spots in the previous presence-only check:
 *   - every push to main tried to push an already published version, so an
 *     expired NUGET_TOKEN failed runs that had nothing to publish;
 *   - an unpublished version (C# 0.5.1-0.6.1, C++ 0.3.43-0.4.1) was never
 *     reported as such, and the C++ release was created anyway.
 *
 * The version check uses the public flat-container index, which needs no key.
 * nuget.org has no read-only endpoint that validates an API key, so an expired
 * key is still only detected by the push itself; push-nuget-package.sh turns
 * that 403 into actionable guidance. Trusted publishing (NUGET_USER) avoids
 * long-lived keys altogether.
 *
 * Environment: PACKAGE_ID, PACKAGE_VERSION (or CSPROJ to read both),
 * GITHUB_TOKEN, NUGET_TOKEN, NUGET_USER, NUGET_INDEX_URL (tests),
 * VERBOSE=1 or RUNNER_DEBUG=1 for diagnostic output.
 * Outputs: publish_required=true|false, auth=trusted-publishing|api-key|none.
 */

import { appendFileSync, readFileSync } from "node:fs";

const defaultIndexUrl = "https://api.nuget.org/v3-flatcontainer";

export const readCsproj = (source) => {
  const read = (name) => new RegExp(`<${name}>([^<]+)</${name}>`).exec(source)?.[1]?.trim();
  return {
    packageId: read("PackageId") ?? read("AssemblyName"),
    version: read("Version") ?? read("VersionPrefix"),
  };
};

export const versionIndexUrl = (packageId, indexUrl = defaultIndexUrl) =>
  `${indexUrl}/${packageId.toLowerCase()}/index.json`;

/**
 * @returns {Promise<{ published: boolean | undefined, detail: string }>}
 * `published` is undefined when nuget.org could not be queried.
 */
export const fetchPublished = async ({ packageId, version, indexUrl, fetchImpl = fetch }) => {
  const url = versionIndexUrl(packageId, indexUrl);
  try {
    const response = await fetchImpl(url, { signal: AbortSignal.timeout(30_000) });
    if (response.status === 404) {
      return { published: false, detail: `${url} returned 404` };
    }
    if (!response.ok) {
      return { published: undefined, detail: `${url} returned ${response.status}` };
    }
    const { versions = [] } = await response.json();
    return {
      published: versions.includes(version.toLowerCase()),
      detail: `${url} lists ${versions.length} versions`,
    };
  } catch (error) {
    return { published: undefined, detail: `${url} failed: ${error.message}` };
  }
};

export const evaluateRelease = ({ published, githubToken, nugetToken, nugetUser }) => {
  const failures = [];
  const warnings = [];

  if (!githubToken) {
    failures.push("GITHUB_TOKEN is unavailable");
  }
  if (published === undefined) {
    warnings.push(
      "nuget.org could not be queried; the push will run with --skip-duplicate",
    );
  }

  const publishRequired = published !== true;
  let auth = "none";
  if (nugetUser) {
    auth = "trusted-publishing";
  } else if (nugetToken) {
    auth = "api-key";
  }

  if (publishRequired && auth === "none") {
    failures.push(
      "The version is not on nuget.org but neither the NUGET_USER variable (trusted publishing) nor the NUGET_TOKEN secret is configured",
    );
  }

  return { passed: failures.length === 0, publishRequired, auth, failures, warnings };
};

const setOutput = (environment, name, value) => {
  console.log(`${name}=${value}`);
  if (environment.GITHUB_OUTPUT) {
    appendFileSync(environment.GITHUB_OUTPUT, `${name}=${value}\n`);
  }
};

export const run = async (environment = process.env, fetchImpl = fetch) => {
  const verbose = environment.VERBOSE === "1" || environment.RUNNER_DEBUG === "1";
  let packageId = environment.PACKAGE_ID;
  let version = environment.PACKAGE_VERSION;
  if (environment.CSPROJ) {
    const project = readCsproj(readFileSync(environment.CSPROJ, "utf8"));
    packageId ||= project.packageId;
    version ||= project.version;
  }
  if (!packageId || !version) {
    console.error("::error title=NuGet release preflight failed::Package id or version is unknown");
    return 1;
  }

  const { published, detail } = await fetchPublished({
    packageId,
    version,
    indexUrl: environment.NUGET_INDEX_URL || defaultIndexUrl,
    fetchImpl,
  });
  if (verbose) {
    console.log(`Version index: ${detail}`);
  }

  const result = evaluateRelease({
    published,
    githubToken: environment.GITHUB_TOKEN ?? "",
    nugetToken: environment.NUGET_TOKEN ?? "",
    nugetUser: environment.NUGET_USER ?? "",
  });

  for (const warning of result.warnings) {
    console.log(`::warning title=NuGet release preflight::${warning}`);
  }
  for (const failure of result.failures) {
    console.error(`::error title=NuGet release preflight failed::${failure}`);
  }

  if (result.publishRequired) {
    console.log(`${packageId} ${version} is not on nuget.org yet; it will be published using ${result.auth}.`);
    if (result.auth === "api-key") {
      console.log(
        "NUGET_TOKEN presence is verified; nuget.org exposes no read-only endpoint for validating expiry or package scope.",
      );
    }
  } else {
    console.log(`${packageId} ${version} is already on nuget.org; publishing will be skipped.`);
  }

  setOutput(environment, "publish_required", String(result.publishRequired));
  setOutput(environment, "auth", result.auth);
  return result.passed ? 0 : 1;
};

if (process.argv[1] && import.meta.url === new URL(`file://${process.argv[1]}`).href) {
  process.exitCode = await run();
}
