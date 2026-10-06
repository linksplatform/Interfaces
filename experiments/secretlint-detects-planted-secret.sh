#!/usr/bin/env bash
# Proves that the secretlint scan in .github/workflows/secrets.yml is not a
# false negative: a planted, well-formed (but fake) GitHub token must fail it.
set -euo pipefail
cd "$(dirname "$0")/.."
planted="experiments/planted-secret.tmp.txt"
trap 'rm -f "$planted"' EXIT
# Split so this script itself does not contain a token-shaped string.
printf 'token = %s%s\n' "ghp_" "wWPw5k4aXcaT4fNP0UcnZwJUVFk6LO0pINUx" > "$planted"
if npx --yes -p secretlint@13.0.7 -p @secretlint/secretlint-rule-preset-recommend@13.0.7 \
    secretlint "$planted"; then
  echo "FAIL: secretlint did not detect the planted token" >&2
  exit 1
fi
echo "OK: secretlint detected the planted token"
