#!/bin/bash

# Script to convert GitHub Actions workflows in linksplatform/Interfaces from nuget.exe (with Mono) to dotnet CLI
# Assumes workflows are in .github/workflows/*.yml or *.yaml
# Targets nuget.exe push commands and replaces them with dotnet nuget push
# Ensures actions/setup-dotnet is included for .NET SDK setup

# Exit on error
set -e

# Directory containing GitHub Actions workflows
WORKFLOW_DIR=".github/workflows"

# Check if workflow directory exists
if [ ! -d "$WORKFLOW_DIR" ]; then
  echo "Error: Workflow directory $WORKFLOW_DIR not found. Please run this script from the repository root."
  exit 1
fi

# Find all .yml and .yaml files in the workflow directory
WORKFLOW_FILES=$(find "$WORKFLOW_DIR" -type f \( -name "*.yml" -o -name "*.yaml" \))

# Check if any workflow files exist
if [ -z "$WORKFLOW_FILES" ]; then
  echo "Error: No workflow files (.yml or .yaml) found in $WORKFLOW_DIR."
  exit 1
fi

# Function to check if actions/setup-dotnet is already included
has_setup_dotnet() {
  local file="$1"
  grep -q "uses: actions/setup-dotnet@v[1-4]" "$file"
}

# Function to add actions/setup-dotnet step if missing
add_setup_dotnet() {
  local file="$1"
  # Find the first step in the job (after 'steps:') and insert setup-dotnet before it
  sed -i.bak '/steps:/a\
      - name: Setup .NET\
        uses: actions/setup-dotnet@v4\
        with:\
          dotnet-version: '\''8.0.x'\''\
' "$file"
  echo "Added actions/setup-dotnet@v4 to $file"
}

# Function to process a workflow file
process_workflow() {
  local file="$1"
  echo "Processing workflow file: $file"

  # Create a backup of the original file
  cp "$file" "$file.bak"

  # Replace nuget source Add with dotnet nuget add source (simplified pattern)
  sed -i.bak "s|nuget source Add -Name \"GitHub\" -Source \"https://nuget.pkg.github.com/linksplatform/index.json\" -UserName linksplatform -Password \${{ secrets.GITHUB_TOKEN }}|dotnet nuget add source https://nuget.pkg.github.com/linksplatform/index.json --name GitHub --username linksplatform --password \${{ secrets.GITHUB_TOKEN }} --store-password-in-clear-text|g" "$file"

  # Replace nuget push with dotnet nuget push
  sed -i.bak -E "s|nuget push ([^[:space:]]+) -Source \"([^\"]+)\" -SkipDuplicate|dotnet nuget push \1 --source \2 --skip-duplicate|g" "$file"

  # Replace nuget.exe push with dotnet nuget push (more general pattern)
  sed -i.bak -E "s|run:[[:space:]]*(mono[[:space:]]+)?/?([a-zA-Z0-9/._-]*\/)?nuget(\.exe)?[[:space:]]+push[[:space:]]+([^[:space:]].*nupkg)([[:space:]]+--api-key[[:space:]]+\$\{\{[[:space:]]*[a-zA-Z0-9._-]+\}\})?([[:space:]]+--source[[:space:]]+[a-zA-Z0-9:/._-]+)?|run: dotnet nuget push \4 --api-key \5 --source \6 --skip-duplicate|g" "$file"

  # Replace nuget.exe restore with dotnet restore (if present)
  sed -i.bak -E "s|run:[[:space:]]*(mono[[:space:]]+)?/?([a-zA-Z0-9/._-]*\/)?nuget(\.exe)?[[:space:]]+restore[[:space:]]+([^[:space:]].*)|run: dotnet restore \4|g" "$file"

  # Remove nuget/setup-nuget@v1 action as it's no longer needed
  sed -i.bak -E "/- uses: nuget\/setup-nuget@v1/d" "$file"

  # Check for changes
  if ! diff "$file" "$file.bak" >/dev/null; then
    echo "Modified $file to use dotnet CLI instead of nuget.exe"
  else
    echo "No nuget.exe commands found in $file; no changes made"
    rm "$file.bak" # Remove backup if no changes
  fi

  # Add actions/setup-dotnet if not present
  if ! has_setup_dotnet "$file"; then
    add_setup_dotnet "$file"
  fi
}

# Process each workflow file
for file in $WORKFLOW_FILES; do
  process_workflow "$file"
done

# Clean up any remaining backup files if no errors
echo "Cleaning up backup files..."
find "$WORKFLOW_DIR" -name "*.bak" -exec rm {} \;

echo "Conversion complete! Please review changes in $WORKFLOW_DIR and test the updated workflows."
echo "If publishing to nuget.org, ensure NUGET_API_KEY is set in GitHub Secrets."
echo "If targeting a different NuGet feed (e.g., GitHub Packages), update the --source URL and authentication as needed."