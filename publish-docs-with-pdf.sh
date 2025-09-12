#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

# Settings
TARGET_BRANCH="gh-pages"
SHA=$(git rev-parse --verify HEAD)
COMMIT_USER_NAME="linksplatform"
COMMIT_USER_EMAIL="linksplatformtechnologies@gmail.com"
REPOSITORY="github.com/linksplatform/$REPOSITORY_NAME"

# Clone the existing gh-pages for this repo into out/
# Create a new empty branch if gh-pages doesn't exist yet (should only happen on first deploy)
git clone "https://$REPOSITORY" out
cd out || exit
git checkout $TARGET_BRANCH || git checkout --orphan $TARGET_BRANCH
cd ..

mkdir -p out/csharp

# Clean out existing contents
rm -rf out/csharp/**/* || exit 0

# Copy generated docs site (includes PDF files)
cp -r _site/* out/csharp/

cd out/csharp || exit

# Do not use index.md
cp README.html index.html

# Enter repository's folder
cd ..

# Now let's go have some fun with the cloned repo
git config user.name "$COMMIT_USER_NAME"
git config user.email "$COMMIT_USER_EMAIL"
git remote rm origin
git remote add origin "https://linksplatform:$GITHUB_TOKEN@$REPOSITORY.git"

# Commit the "changes", i.e. the new version.
# The delta will show diffs between new and old versions.
git add --all
git commit -m "Deploy to GitHub Pages with PDF documentation: $SHA"

# Now that we're all set up, we can push.
git push "https://linksplatform:$GITHUB_TOKEN@$REPOSITORY.git" "$TARGET_BRANCH"
cd ..

# Clean up
rm -rf out
rm -rf _site
rm -rf _site_pdf