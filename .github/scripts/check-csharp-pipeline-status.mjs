#!/usr/bin/env node

const allJobNames = [
  "test",
  "releasePreflight",
  "pushNuGetPackageToGitHubPackageRegistry",
  "pushToNuget",
  "publishRelease",
  "findChangedCsFiles",
  "generatePdfWithCode",
  "buildDocumentation",
  "publishDocumentation",
];

export const evaluatePipeline = ({ eventName, documentationChanged, needs }) => {
  const required = new Set(["test", "findChangedCsFiles"]);

  if (documentationChanged) {
    required.add("generatePdfWithCode");
    required.add("buildDocumentation");
    required.add("publishDocumentation");
  }

  if (eventName === "push") {
    required.add("releasePreflight");
    required.add("pushNuGetPackageToGitHubPackageRegistry");
    required.add("pushToNuget");
    required.add("publishRelease");
  }

  const failures = [];
  for (const name of allJobNames) {
    const result = needs[name]?.result;
    if (!result) {
      failures.push(`${name}: result is missing`);
    } else if (required.has(name) && result !== "success") {
      failures.push(`${name}: required job finished with ${result}`);
    } else if (!required.has(name) && !["success", "skipped"].includes(result)) {
      failures.push(`${name}: optional job finished with ${result}`);
    }
  }

  return { passed: failures.length === 0, failures };
};

export const run = (environment = process.env) => {
  let needs;
  try {
    needs = JSON.parse(environment.NEEDS_JSON ?? "");
  } catch (error) {
    console.error(`::error title=C# pipeline status invalid::${error.message}`);
    return 1;
  }

  const result = evaluatePipeline({
    eventName: environment.EVENT_NAME ?? "",
    documentationChanged: environment.DOCUMENTATION_CHANGED === "true",
    needs,
  });

  for (const failure of result.failures) {
    console.error(`::error title=C# pipeline failed::${failure}`);
  }
  if (result.passed) {
    console.log("Every required C# pipeline job succeeded.");
  }
  return result.passed ? 0 : 1;
};

if (process.argv[1] && import.meta.url === new URL(`file://${process.argv[1]}`).href) {
  process.exitCode = run();
}
