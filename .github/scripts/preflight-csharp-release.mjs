#!/usr/bin/env node

/**
 * Check the release credentials that can be verified without publishing.
 * NuGet.org has no read-only API-key validation endpoint, so this can prove
 * only that NUGET_TOKEN is present. An expired or incorrectly scoped key is
 * still reported by `dotnet nuget push`, and release creation remains gated
 * on that command succeeding.
 */

export const evaluateCredentials = ({ githubToken, nugetToken }) => {
  const failures = [];

  if (!githubToken) {
    failures.push("GITHUB_TOKEN is unavailable");
  }
  if (!nugetToken) {
    failures.push("NUGET_TOKEN is not configured");
  }

  return { passed: failures.length === 0, failures };
};

export const run = (environment = process.env) => {
  const result = evaluateCredentials({
    githubToken: environment.GITHUB_TOKEN ?? "",
    nugetToken: environment.NUGET_TOKEN ?? "",
  });

  for (const failure of result.failures) {
    console.error(`::error title=C# release preflight failed::${failure}`);
  }

  if (!result.passed) {
    return 1;
  }

  console.log("C# release credentials are configured.");
  console.log(
    "NuGet token presence is verified; NuGet.org exposes no read-only endpoint for validating expiry or package scope.",
  );
  return 0;
};

if (process.argv[1] && import.meta.url === new URL(`file://${process.argv[1]}`).href) {
  process.exitCode = run();
}
