#!/usr/bin/env bash

set -euo pipefail

site=${1:?Usage: validate-csharp-docs.sh SITE_DIRECTORY}

for file in \
  index.html \
  api/Platform.Interfaces.html \
  api/Platform.Interfaces.IFactory-1.html \
  xrefmap.yml; do
  if [[ ! -s "$site/$file" ]]; then
    echo "Documentation site is missing $site/$file." >&2
    exit 1
  fi
done

echo "Validated DocFX home page, API pages, and cross-reference map."
