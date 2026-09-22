#!/usr/bin/env node

/**
 * Verifies that every GitHub Actions badge in README.md points at a workflow
 * that actually exists in .github/workflows.
 *
 * Background (issue #141): the C++ badge used
 *   https://github.com/linksplatform/Interfaces/workflows/deploy-cpp/badge.svg
 * The path segment after /workflows/ must be the workflow *name* (the `name:`
 * field inside the YAML file), not the file name. No workflow is named
 * "deploy-cpp" (deploy-cpp.yml is named "Deploy new cpp version"), so GitHub
 * answered 404 and the badge rendered as a broken image.
 *
 * Usage:
 *   node .github/scripts/check-readme-badges.mjs [--verbose]
 *     [--readme <path>] [--workflows <directory>]
 *
 * --readme and --workflows default to this repository's README.md and
 * .github/workflows, and exist so the test suite can point the check at
 * fixtures.
 *
 * Exits with code 1 and a description of every unresolved badge when a badge
 * references a workflow that does not exist.
 */

import { readFileSync, readdirSync, existsSync } from "fs";
import { join, dirname, resolve } from "path";
import { fileURLToPath } from "url";

const __dirname = dirname(fileURLToPath(import.meta.url));
const repositoryRoot = resolve(__dirname, "..", "..");

const readOption = (option, fallback) => {
  const index = process.argv.indexOf(option);
  return index === -1 || index === process.argv.length - 1
    ? fallback
    : resolve(process.argv[index + 1]);
};

const workflowsDirectory = readOption("--workflows", join(repositoryRoot, ".github", "workflows"));
const readmePath = readOption("--readme", join(repositoryRoot, "README.md"));

const verbose = process.argv.includes("--verbose");
const trace = (...args) => {
  if (verbose) {
    console.log("[trace]", ...args);
  }
};

/** Reads the top-level `name:` of a workflow file, unquoting it when needed. */
const readWorkflowName = (filePath) => {
  const nameLine = readFileSync(filePath, "utf8")
    .split(/\r?\n/)
    .find((line) => /^name:\s*\S/.test(line));
  if (!nameLine) {
    return null;
  }
  return nameLine.replace(/^name:\s*/, "").trim().replace(/^["'](.*)["']$/, "$1");
};

const collectWorkflows = () => {
  if (!existsSync(workflowsDirectory)) {
    return [];
  }
  return readdirSync(workflowsDirectory)
    .filter((fileName) => /\.ya?ml$/.test(fileName))
    .map((fileName) => ({
      fileName,
      name: readWorkflowName(join(workflowsDirectory, fileName)),
    }));
};

/**
 * `actions?workflow=Deploy+new+cpp+version` encodes spaces as `+`, while
 * `workflows/Deploy%20new%20cpp%20version/badge.svg` encodes them as `%20`.
 */
const decodeWorkflowReference = (value, isQueryParameter) =>
  decodeURIComponent(isQueryParameter ? value.replace(/\+/g, "%20") : value);

/** Every way README.md can refer to a workflow of this repository. */
const referencePatterns = [
  {
    kind: "badge (workflow name)",
    pattern: /https:\/\/github\.com\/[^/\s)]+\/[^/\s)]+\/workflows\/([^/\s)]+)\/badge\.svg/g,
    resolve: (match) => ({ by: "name", value: decodeWorkflowReference(match[1], false) }),
  },
  {
    kind: "link (workflow name)",
    pattern: /https:\/\/github\.com\/[^/\s)]+\/[^/\s)]+\/actions\?workflow=([^\s)]+)/g,
    resolve: (match) => ({ by: "name", value: decodeWorkflowReference(match[1], true) }),
  },
  {
    // Covers both the badge form (.../actions/workflows/<file>/badge.svg) and the
    // plain link form (.../actions/workflows/<file>). The trailing boundary keeps
    // the capture from backtracking into a truncated file name such as "cpp-test.ym".
    kind: "workflow file",
    pattern: /https:\/\/github\.com\/[^/\s)]+\/[^/\s)]+\/actions\/workflows\/([^/\s)?#]+)(\/badge\.svg)?(?=[\s)?#]|$)/g,
    resolve: (match) => ({
      kind: match[2] ? "badge (workflow file)" : "link (workflow file)",
      by: "file",
      value: decodeWorkflowReference(match[1], false),
    }),
  },
];

const collectReferences = (readme) => {
  const references = [];
  for (const { kind, pattern, resolve: resolveReference } of referencePatterns) {
    for (const match of readme.matchAll(pattern)) {
      references.push({ kind, url: match[0], ...resolveReference(match) });
    }
  }
  return references;
};

const main = () => {
  const workflows = collectWorkflows();
  const readme = readFileSync(readmePath, "utf8");
  const references = collectReferences(readme);

  trace(`workflows directory: ${workflowsDirectory}`);
  for (const workflow of workflows) {
    trace(`workflow ${workflow.fileName} -> name ${JSON.stringify(workflow.name)}`);
  }
  trace(`found ${references.length} workflow reference(s) in README.md`);

  const knownNames = workflows.map((workflow) => workflow.name).filter(Boolean);
  const knownFiles = workflows.map((workflow) => workflow.fileName);

  const failures = [];
  for (const reference of references) {
    const known = reference.by === "name" ? knownNames : knownFiles;
    const resolved = known.includes(reference.value);
    trace(`${resolved ? "ok" : "FAIL"} ${reference.kind} by ${reference.by}: ${JSON.stringify(reference.value)}`);
    if (!resolved) {
      failures.push(reference);
    }
  }

  if (references.length === 0) {
    console.error("No GitHub Actions badges found in README.md, the check would pass vacuously.");
    process.exit(1);
  }

  if (failures.length > 0) {
    console.error(`README.md references ${failures.length} workflow(s) that do not exist:\n`);
    for (const failure of failures) {
      console.error(`  ${failure.kind}: ${failure.url}`);
      console.error(`    referenced workflow ${failure.by}: ${JSON.stringify(failure.value)}`);
    }
    console.error(`\nAvailable workflow names: ${knownNames.map((name) => JSON.stringify(name)).join(", ")}`);
    console.error(`Available workflow files: ${knownFiles.join(", ")}`);
    console.error("\nThe segment after /workflows/ is the workflow `name:`, not the file name.");
    process.exit(1);
  }

  console.log(`All ${references.length} workflow reference(s) in README.md resolve to existing workflows.`);
};

main();
