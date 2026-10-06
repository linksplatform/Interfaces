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
 * An API key is checked without publishing anything, through the same two
 * calls that `nuget push` makes for symbol packages (NuGetGallery
 * ApiController): POST /api/v2/package/create-verification-key/<id> rejects an
 * invalid, expired or non-push key with 401/403, and GET
 * /api/v2/verifykey/<id>/<published version> with the returned one-time key
 * rejects a key whose glob or owner does not cover the package. The gallery
 * deletes the one-time key after that call. Trusted publishing (NUGET_USER)
 * avoids long-lived keys altogether.
 *
 * Environment: PACKAGE_ID, PACKAGE_VERSION (or CSPROJ to read both),
 * GITHUB_TOKEN, NUGET_TOKEN, NUGET_USER, NUGET_INDEX_URL and NUGET_GALLERY_URL
 * (tests), VERBOSE=1 or RUNNER_DEBUG=1 for diagnostic output.
 * Outputs: publish_required=true|false, auth=trusted-publishing|api-key|none.
 */

import { appendFileSync, readFileSync } from "node:fs";

const defaultIndexUrl = "https://api.nuget.org/v3-flatcontainer";
const defaultGalleryUrl = "https://www.nuget.org";

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
 * @returns {Promise<{ published: boolean | undefined, latest?: string, detail: string }>}
 * `published` is undefined when nuget.org could not be queried; `latest` is
 * the newest published version, if any.
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
      latest: versions.at(-1),
      detail: `${url} lists ${versions.length} versions`,
    };
  } catch (error) {
    return { published: undefined, detail: `${url} failed: ${error.message}` };
  }
};

/**
 * @returns {Promise<{ valid: boolean | undefined, detail: string }>}
 * `valid` is undefined when nuget.org could not answer, and also when no
 * version is published yet: then only the key itself can be checked, not
 * whether it may push this package.
 */
export const verifyApiKey = async ({
  packageId,
  publishedVersion,
  apiKey,
  galleryUrl = defaultGalleryUrl,
  fetchImpl = fetch,
}) => {
  const id = encodeURIComponent(packageId);
  const createUrl = `${galleryUrl}/api/v2/package/create-verification-key/${id}`;
  let key;
  try {
    const response = await fetchImpl(createUrl, {
      method: "POST",
      headers: { "X-NuGet-ApiKey": apiKey },
      body: "",
      signal: AbortSignal.timeout(30_000),
    });
    if (response.status === 401 || response.status === 403) {
      return { valid: false, detail: `${createUrl} returned ${response.status}` };
    }
    if (!response.ok) {
      return { valid: undefined, detail: `${createUrl} returned ${response.status}` };
    }
    key = (await response.json()).Key;
  } catch (error) {
    return { valid: undefined, detail: `${createUrl} failed: ${error.message}` };
  }
  if (!key) {
    return { valid: undefined, detail: `${createUrl} returned no verification key` };
  }
  if (!publishedVersion) {
    return {
      valid: undefined,
      detail: `${createUrl} accepted the key, but no published version exists to check its package scope`,
    };
  }

  const verifyUrl = `${galleryUrl}/api/v2/verifykey/${id}/${encodeURIComponent(publishedVersion)}`;
  try {
    const response = await fetchImpl(verifyUrl, {
      headers: { "X-NuGet-ApiKey": key },
      signal: AbortSignal.timeout(30_000),
    });
    if (response.status === 401 || response.status === 403) {
      return { valid: false, detail: `${verifyUrl} returned ${response.status}` };
    }
    return {
      valid: response.ok ? true : undefined,
      detail: `${verifyUrl} returned ${response.status}`,
    };
  } catch (error) {
    return { valid: undefined, detail: `${verifyUrl} failed: ${error.message}` };
  }
};

export const evaluateRelease = ({ published, githubToken, nugetToken, nugetUser, keyValid, keyDetail }) => {
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
  if (publishRequired && auth === "api-key") {
    if (keyValid === false) {
      failures.push(
        "nuget.org rejected the NUGET_TOKEN secret: it is invalid, expired, or not scoped to push this package. Create a new key at https://www.nuget.org/account/apikeys with push scope for the package and store it as the NUGET_TOKEN repository secret, or configure trusted publishing (https://learn.microsoft.com/nuget/nuget-org/trusted-publishing) and set the NUGET_USER repository variable",
      );
    } else if (keyValid === undefined) {
      const reason = keyDetail ? ` (${keyDetail})` : "";
      warnings.push(`nuget.org could not verify the NUGET_TOKEN secret${reason}; the push will show whether it is accepted`);
    }
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

  const { published, latest, detail } = await fetchPublished({
    packageId,
    version,
    indexUrl: environment.NUGET_INDEX_URL || defaultIndexUrl,
    fetchImpl,
  });
  if (verbose) {
    console.log(`Version index: ${detail}`);
  }

  const nugetToken = environment.NUGET_TOKEN ?? "";
  const nugetUser = environment.NUGET_USER ?? "";
  let keyValid;
  let keyDetail;
  if (published !== true && nugetToken && !nugetUser) {
    const verification = await verifyApiKey({
      packageId,
      publishedVersion: latest,
      apiKey: nugetToken,
      galleryUrl: environment.NUGET_GALLERY_URL || defaultGalleryUrl,
      fetchImpl,
    });
    keyValid = verification.valid;
    keyDetail = verification.detail;
    if (verbose) {
      console.log(`API key verification: ${verification.detail}`);
    }
  }

  const result = evaluateRelease({
    published,
    githubToken: environment.GITHUB_TOKEN ?? "",
    nugetToken,
    nugetUser,
    keyValid,
    keyDetail,
  });

  for (const warning of result.warnings) {
    console.log(`::warning title=NuGet release preflight::${warning}`);
  }
  for (const failure of result.failures) {
    console.error(`::error title=NuGet release preflight failed::${failure}`);
  }

  if (result.publishRequired) {
    console.log(`${packageId} ${version} is not on nuget.org yet; it will be published using ${result.auth}.`);
    if (result.auth === "api-key" && keyValid) {
      console.log("nuget.org accepted NUGET_TOKEN for pushing this package.");
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
