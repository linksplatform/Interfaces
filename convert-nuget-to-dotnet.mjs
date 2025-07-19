#!/usr/bin/env bun

/**
 * Script to convert GitHub Actions workflows from nuget.exe (with Mono) to dotnet CLI
 * Assumes workflows are in .github/workflows/*.yml or *.yaml
 * Targets nuget.exe push commands and replaces them with dotnet nuget push
 * Ensures actions/setup-dotnet is included for .NET SDK setup
 */

import { readFileSync, writeFileSync, existsSync, readdirSync, unlinkSync } from 'fs';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Exit on error
process.on('uncaughtException', (err) => {
  console.error('Error:', err.message);
  process.exit(1);
});

// Directory containing GitHub Actions workflows
const WORKFLOW_DIR = '.github/workflows';

// Check if workflow directory exists
if (!existsSync(WORKFLOW_DIR)) {
  console.error(`Error: Workflow directory ${WORKFLOW_DIR} not found. Please run this script from the repository root.`);
  process.exit(1);
}

// Find all .yml and .yaml files in the workflow directory
const workflowFiles = readdirSync(WORKFLOW_DIR)
  .filter(file => file.endsWith('.yml') || file.endsWith('.yaml'))
  .map(file => join(WORKFLOW_DIR, file));

if (workflowFiles.length === 0) {
  console.error(`Error: No workflow files (.yml or .yaml) found in ${WORKFLOW_DIR}.`);
  process.exit(1);
}

/**
 * Check if actions/setup-dotnet is already included
 */
function hasSetupDotnet(content) {
  return content.includes('uses: actions/setup-dotnet@v');
}

/**
 * Add actions/setup-dotnet step if missing
 */
function addSetupDotnet(content) {
  const setupDotnetStep = `      - name: Setup .NET
        uses: actions/setup-dotnet@v4
        with:
          dotnet-version: '8.0.x'
`;
  
  // Find the first steps section and add setup-dotnet after it
  const stepsIndex = content.indexOf('steps:');
  if (stepsIndex !== -1) {
    const beforeSteps = content.substring(0, stepsIndex);
    const afterSteps = content.substring(stepsIndex);
    
    // Find the first step after 'steps:'
    const firstStepIndex = afterSteps.indexOf('\n      -');
    if (firstStepIndex !== -1) {
      return beforeSteps + afterSteps.substring(0, firstStepIndex) + '\n' + setupDotnetStep + afterSteps.substring(firstStepIndex);
    } else {
      // No steps found, add after 'steps:'
      return beforeSteps + afterSteps + '\n' + setupDotnetStep;
    }
  }
  
  return content;
}

/**
 * Process a workflow file
 */
async function processWorkflow(filePath) {
  console.log(`Processing workflow file: ${filePath}`);
  
  // Read the file
  let content = readFileSync(filePath, 'utf8');
  const originalContent = content;
  
  // Create backup
  writeFileSync(`${filePath}.bak`, content);
  
  // Replace nuget source Add with dotnet nuget add source
  content = content.replace(
    /nuget source Add -Name "GitHub" -Source "https:\/\/nuget\.pkg\.github\.com\/linksplatform\/index\.json" -UserName linksplatform -Password \${{ secrets\.GITHUB_TOKEN }}/g,
    'dotnet nuget add source https://nuget.pkg.github.com/linksplatform/index.json --name GitHub --username linksplatform --password ${{ secrets.GITHUB_TOKEN }} --store-password-in-clear-text'
  );
  
  // Replace nuget push with dotnet nuget push
  content = content.replace(
    /nuget push ([^ ]+) -Source "([^"]+)" -SkipDuplicate/g,
    'dotnet nuget push $1 --source $2 --skip-duplicate'
  );
  
  // Replace nuget.exe push with dotnet nuget push (more general pattern)
  content = content.replace(
    /run:\s*(mono\s+)?\/?([a-zA-Z0-9\/._-]*\/)?nuget(\.exe)?\s+push\s+([^ ].*nupkg)(\s+--api-key\s+\$\{\{[^}]+\}\})?(\s+--source\s+[a-zA-Z0-9:\/._-]+)?/g,
    'run: dotnet nuget push $4 --api-key $5 --source $6 --skip-duplicate'
  );
  
  // Replace nuget.exe restore with dotnet restore (if present)
  content = content.replace(
    /run:\s*(mono\s+)?\/?([a-zA-Z0-9\/._-]*\/)?nuget(\.exe)?\s+restore\s+([^ ].*)/g,
    'run: dotnet restore $4'
  );
  
  // Remove nuget/setup-nuget@v1 action as it's no longer needed
  content = content.replace(/- uses: nuget\/setup-nuget@v1\n/g, '');
  
  // Check for changes
  if (content !== originalContent) {
    console.log(`Modified ${filePath} to use dotnet CLI instead of nuget.exe`);
    writeFileSync(filePath, content);
  } else {
    console.log(`No nuget.exe commands found in ${filePath}; no changes made`);
    // Remove backup if no changes
    try {
      unlinkSync(`${filePath}.bak`);
    } catch (error) {
      // Backup file might not exist, ignore error
    }
  }
  
  // Add actions/setup-dotnet if not present
  if (!hasSetupDotnet(content)) {
    content = addSetupDotnet(content);
    writeFileSync(filePath, content);
    console.log(`Added actions/setup-dotnet@v4 to ${filePath}`);
  }
}

/**
 * Main function
 */
async function main() {
  console.log('Starting nuget.exe to dotnet CLI conversion...\n');
  
  // Process each workflow file
  for (const file of workflowFiles) {
    await processWorkflow(file);
    console.log('');
  }
  
  // Clean up any remaining backup files
  console.log('Cleaning up backup files...');
  for (const file of workflowFiles) {
    const backupFile = `${file}.bak`;
    if (existsSync(backupFile)) {
      try {
        unlinkSync(backupFile);
      } catch (error) {
        console.log(`Warning: Could not remove backup file ${backupFile}: ${error.message}`);
      }
    }
  }
  
  console.log('\n✅ Conversion complete! Please review changes in .github/workflows and test the updated workflows.');
  console.log('If publishing to nuget.org, ensure NUGET_API_KEY is set in GitHub Secrets.');
  console.log('If targeting a different NuGet feed (e.g., GitHub Packages), update the --source URL and authentication as needed.');
}

// Run the script
main().catch(error => {
  console.error('Script failed:', error);
  process.exit(1);
});