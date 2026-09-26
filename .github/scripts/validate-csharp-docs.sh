#!/usr/bin/env bash

set -euo pipefail

site=${1:?Usage: validate-csharp-docs.sh SITE_DIRECTORY}

for file in \
  index.html \
  api/Platform.Interfaces.html \
  api/Platform.Interfaces.IFactory-1.html \
  Platform.Interfaces.Documentation.pdf \
  xrefmap.yml; do
  if [[ ! -s "$site/$file" ]]; then
    echo "Documentation site is missing $site/$file." >&2
    exit 1
  fi
done

if [[ $(head -c 5 "$site/Platform.Interfaces.Documentation.pdf") != '%PDF-' ]]; then
  echo "Documentation PDF is invalid: $site/Platform.Interfaces.Documentation.pdf." >&2
  exit 1
fi

if ! grep -Fq 'Platform.Interfaces.Documentation.pdf' "$site/index.html"; then
  echo "Documentation home page is missing its PDF link: $site/index.html." >&2
  exit 1
fi

echo "Validated DocFX home page, API pages, documentation PDF, and cross-reference map."
