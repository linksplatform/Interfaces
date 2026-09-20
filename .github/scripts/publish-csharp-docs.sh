#!/usr/bin/env bash

set -euo pipefail

if [[ -z ${GITHUB_REPOSITORY:-} || -z ${GITHUB_TOKEN:-} ]]; then
  echo "GITHUB_REPOSITORY and GITHUB_TOKEN are required." >&2
  exit 1
fi

site_directory=${1:-_site}
if [[ ! -f "$site_directory/index.html" || ! -f "$site_directory/Platform.Interfaces.pdf" ]]; then
  echo "$site_directory must contain index.html and Platform.Interfaces.pdf." >&2
  exit 1
fi

site_directory=$(cd "$site_directory" && pwd)
deploy_directory=$(mktemp -d)

cleanup() {
  rm -rf "$deploy_directory"
}
trap cleanup EXIT

authorization=$(printf 'x-access-token:%s' "$GITHUB_TOKEN" | base64 -w0)
repository_url="https://github.com/${GITHUB_REPOSITORY}.git"
git -c http.extraheader="AUTHORIZATION: basic $authorization" clone \
  --branch gh-pages --depth 1 "$repository_url" "$deploy_directory"

destination="$deploy_directory/csharp"
mkdir -p "$destination"
find "$destination" -mindepth 1 -delete
cp -a "$site_directory/." "$destination/"

git -C "$deploy_directory" config user.name linksplatform
git -C "$deploy_directory" config user.email linksplatformtechnologies@gmail.com
git -C "$deploy_directory" add --all

if git -C "$deploy_directory" diff --cached --quiet; then
  echo "Documentation is already current."
  exit 0
fi

git -C "$deploy_directory" commit -m "Deploy C# documentation: ${GITHUB_SHA:-unknown}"
git -C "$deploy_directory" -c http.extraheader="AUTHORIZATION: basic $authorization" \
  push origin HEAD:gh-pages
