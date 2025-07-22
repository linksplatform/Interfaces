#!/usr/bin/env bun

/**
 * Script to convert GitHub Actions workflows from nuget.exe (with Mono) to dotnet CLI
 * Assumes workflows are in .github/workflows/*.yml or *.yaml
 * Uses the 'yaml' package to parse, process, and stringify the workflow files
 */

import { readFileSync, writeFileSync, existsSync, readdirSync, unlinkSync } from 'fs';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

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

console.log('Starting nuget.exe to dotnet CLI conversion...\n');

// Load YAML library using use-m
const { use } = eval(
  await fetch('https://unpkg.com/use-m/use.js').then(u => u.text())
);

const yaml = await use('yaml');
console.log('✅ Loaded YAML library successfully');

// Process each workflow file
for (const filePath of workflowFiles) {
  console.log(`Processing workflow file: ${filePath}`);
  
  const content = readFileSync(filePath, 'utf8');
  writeFileSync(`${filePath}.bak`, content);
  
  // Parse YAML into a JavaScript object
  const workflow = yaml.parse(content);
  
  // Function to check if a step uses dotnet commands
  const usesDotnet = (step) => {
    if (step.run) {
      return /dotnet\s+(test|build|pack|restore|nuget)/.test(step.run);
    }
    return false;
  };
  
  // Check if any job uses dotnet commands
  const hasDotnetCommands = Object.values(workflow.jobs || {}).some(job => 
    (job.steps || []).some(usesDotnet)
  );
  
  // Modify steps in each job
  for (const job of Object.values(workflow.jobs || {})) {
    if (job.steps) {
      job.steps = job.steps.map(step => {
        if (step.run) {
          // Replace nuget source Add
          step.run = step.run.replace(
            /nuget source Add -Name "GitHub" -Source "https:\/\/nuget\.pkg\.github\.com\/linksplatform\/index\.json" -UserName linksplatform -Password \${{ secrets\.GITHUB_TOKEN }}/g,
            'dotnet nuget add source https://nuget.pkg.github.com/linksplatform/index.json --name GitHub --username linksplatform --password ${{ secrets.GITHUB_TOKEN }} --store-password-in-clear-text'
          );
          
          // Replace nuget push
          step.run = step.run.replace(
            /nuget push ([^ ]+) -Source "([^"]+)" -SkipDuplicate/g,
            'dotnet nuget push $1 --source $2 --skip-duplicate'
          );
          
          // Replace nuget.exe push (general)
          step.run = step.run.replace(
            /(mono\s+)?\/?([a-zA-Z0-9\/._-]*\/)?nuget(\.exe)?\s+push\s+([^ ].*nupkg)(\s+--api-key\s+\$\{\{[^}]+\}\})?(\s+--source\s+[a-zA-Z0-9:\/._-]+)?/g,
            'dotnet nuget push $4 --api-key $5 --source $6 --skip-duplicate'
          );
          
          // Replace nuget.exe restore
          step.run = step.run.replace(
            /(mono\s+)?\/?([a-zA-Z0-9\/._-]*\/)?nuget(\.exe)?\s+restore\s+([^ ].*)/g,
            'dotnet restore $4'
          );
        }
        return step;
      });
      
      // Remove nuget/setup-nuget@v1 step
      job.steps = job.steps.filter(step => !(step.uses && step.uses.includes('nuget/setup-nuget@v1')));
      
      // Add or remove actions/setup-dotnet
      const hasSetupDotnet = job.steps.some(step => step.uses && step.uses.includes('actions/setup-dotnet@v'));
      if (hasDotnetCommands && !hasSetupDotnet) {
        job.steps.unshift({
          name: 'Setup .NET',
          uses: 'actions/setup-dotnet@v4',
          with: { 'dotnet-version': '8.0.x' }
        });
        console.log(`Added actions/setup-dotnet@v4 to ${filePath}`);
      } else if (!hasDotnetCommands && hasSetupDotnet) {
        job.steps = job.steps.filter(step => !(step.uses && step.uses.includes('actions/setup-dotnet@v')));
        console.log(`Removed unnecessary actions/setup-dotnet@v4 from ${filePath}`);
      }
    }
  }
  
  // Stringify back to YAML
  const newContent = yaml.stringify(workflow);
  
  // Write changes
  writeFileSync(filePath, newContent);
  console.log(`Modified ${filePath} to use dotnet CLI instead of nuget.exe`);
  
  // Remove backup
  unlinkSync(`${filePath}.bak`);
  
  console.log('');
}

console.log('\n✅ Conversion complete! Please review changes in .github/workflows and test the updated workflows.');
console.log('If publishing to nuget.org, ensure NUGET_API_KEY is set in GitHub Secrets.');
console.log('If targeting a different NuGet feed (e.g., GitHub Packages), update the --source URL and authentication as needed.');