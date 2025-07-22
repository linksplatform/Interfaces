#!/usr/bin/env bun

/**
 * Script to convert GitHub Actions workflows from nuget.exe (with Mono) to dotnet CLI
 * Assumes workflows are in .github/workflows/*.yml or *.yaml
 * Uses targeted text replacement to preserve original YAML formatting
 */

import {
  readFileSync,
  writeFileSync,
  existsSync,
  readdirSync,
  unlinkSync,
} from "fs";
import { join, dirname } from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Directory containing GitHub Actions workflows
const WORKFLOW_DIR = ".github/workflows";

// Check if workflow directory exists
if (!existsSync(WORKFLOW_DIR)) {
  console.error(
    `Error: Workflow directory ${WORKFLOW_DIR} not found. Please run this script from the repository root.`,
  );
  process.exit(1);
}

// Find all .yml and .yaml files in the workflow directory
const workflowFiles = readdirSync(WORKFLOW_DIR)
  .filter((file) => file.endsWith(".yml") || file.endsWith(".yaml"))
  .map((file) => join(WORKFLOW_DIR, file));

if (workflowFiles.length === 0) {
  console.error(
    `Error: No workflow files (.yml or .yaml) found in ${WORKFLOW_DIR}.`,
  );
  process.exit(1);
}

console.log("Starting nuget.exe to dotnet CLI conversion...\n");

// Process each workflow file
for (const filePath of workflowFiles) {
  console.log(`Processing workflow file: ${filePath}`);

  const originalContent = readFileSync(filePath, "utf8");
  let content = originalContent;
  let hasChanges = false;

  // Create backup
  writeFileSync(`${filePath}.bak`, originalContent);

  // Check if file contains dotnet commands (to determine if setup-dotnet is needed)
  const hasDotnetCommands = /dotnet\s+(test|build|pack|restore|nuget|run)/.test(
    content,
  );

  // 1. Replace nuget source Add commands
  const nugetSourcePattern =
    /nuget source Add -Name "GitHub" -Source "https:\/\/nuget\.pkg\.github\.com\/linksplatform\/index\.json" -UserName linksplatform -Password \${{ secrets\.GITHUB_TOKEN }}/g;
  if (nugetSourcePattern.test(content)) {
    content = content.replace(
      nugetSourcePattern,
      "dotnet nuget add source https://nuget.pkg.github.com/linksplatform/index.json --name GitHub --username linksplatform --password ${{ secrets.GITHUB_TOKEN }} --store-password-in-clear-text",
    );
    hasChanges = true;
    console.log("  ✓ Replaced nuget source Add command");
  }

  // 2. Replace nuget push commands (specific pattern)
  const nugetPushPattern =
    /nuget push ([^\s]+) -Source "([^"]+)" -SkipDuplicate/g;
  if (nugetPushPattern.test(content)) {
    content = content.replace(
      nugetPushPattern,
      'dotnet nuget push $1 --source "$2" --skip-duplicate',
    );
    hasChanges = true;
    console.log("  ✓ Replaced nuget push commands");
  }

  // 3. Replace mono/nuget.exe push commands (but not dotnet nuget push)
  const monoNugetPattern =
    /^(\s*)(mono\s+)?([^\s]*\/)?nuget(\.exe)?\s+push\s+([^\s\r\n]+)/gm;
  let monoMatches = [...content.matchAll(monoNugetPattern)];

  if (monoMatches.length > 0) {
    for (const match of monoMatches) {
      const [fullMatch, indent, mono, path, exe, restOfLine] = match;
      // Only replace if this line doesn't already start with "dotnet"
      if (!fullMatch.trim().startsWith("dotnet")) {
        const replacement = `${indent}dotnet nuget push ${restOfLine} --skip-duplicate`;
        content = content.replace(fullMatch, replacement);
        hasChanges = true;
      }
    }
    if (hasChanges) {
      console.log("  ✓ Replaced mono nuget.exe push commands");
    }
  }

  // 4. Replace nuget.exe restore commands (but not dotnet restore)
  const nugetRestorePattern =
    /^(\s*)(mono\s+)?([^\s]*\/)?nuget(\.exe)?\s+restore\s+([^\r\n]+)/gm;
  let restoreMatches = [...content.matchAll(nugetRestorePattern)];

  if (restoreMatches.length > 0) {
    for (const match of restoreMatches) {
      const [fullMatch, indent, mono, path, exe, restOfLine] = match;
      // Only replace if this line doesn't already start with "dotnet"
      if (!fullMatch.trim().startsWith("dotnet")) {
        const replacement = `${indent}dotnet restore ${restOfLine}`;
        content = content.replace(fullMatch, replacement);
        hasChanges = true;
      }
    }
    if (hasChanges) {
      console.log("  ✓ Replaced nuget restore commands");
    }
  }

  // 5. Remove nuget/setup-nuget@v1 steps (entire step block)
  const setupNugetPattern =
    /^(\s+)- name: Setup NuGet[\s\S]*?uses:\s+nuget\/setup-nuget@v1[\s\S]*?(?=^\s+- |\n\s*$|\n[^\s])/gm;
  if (setupNugetPattern.test(content)) {
    content = content.replace(setupNugetPattern, "");
    hasChanges = true;
    console.log("  ✓ Removed nuget/setup-nuget@v1 step");
  }

  // Alternative pattern for setup-nuget removal (just the uses line and with block)
  const setupNugetSimplePattern =
    /^(\s+)uses: nuget\/setup-nuget@v1\s*\n(\s+with:[\s\S]*?(?=\n\s+[^\s]|\n\s*$|\n[^\s]))?/gm;
  if (setupNugetSimplePattern.test(content)) {
    content = content.replace(setupNugetSimplePattern, "");
    hasChanges = true;
    console.log("  ✓ Removed nuget/setup-nuget@v1 usage");
  }

  // 6. Check if setup-dotnet is needed (informational only)
  const hasSetupDotnet = /uses:\s+actions\/setup-dotnet@v/.test(content);

  if (hasDotnetCommands && !hasSetupDotnet) {
    console.log(
      "  ℹ️  Note: This workflow uses dotnet commands but doesn't have actions/setup-dotnet@v4",
    );
    console.log(
      "     You may need to add it manually to ensure .NET is available",
    );
  }

  // Only write file if there were actual changes
  if (hasChanges) {
    writeFileSync(filePath, content);
    console.log(`  ✅ Modified ${filePath}`);
  } else {
    console.log(`  ➡️  No changes needed for ${filePath}`);
  }

  // Remove backup file
  unlinkSync(`${filePath}.bak`);

  console.log("");
}

console.log(
  "\n✅ Conversion complete! Please review changes and test the updated workflows.",
);
console.log(
  "If publishing to nuget.org, ensure NUGET_API_KEY is set in GitHub Secrets.",
);
console.log(
  "If targeting a different NuGet feed (e.g., GitHub Packages), update the --source URL and authentication as needed.",
);
