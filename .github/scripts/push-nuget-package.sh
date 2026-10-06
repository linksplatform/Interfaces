#!/usr/bin/env bash
# Push one package to nuget.org, or skip when the release preflight found the
# version already published. A rejected key only surfaced as a bare
# "403 (The specified API key is invalid, has expired, ...)" line (issue #150);
# this prints what the maintainer has to change instead.
#
# Usage: push-nuget-package.sh <package.nupkg>
# Environment:
#   NUGET_API_KEY      short-lived trusted-publishing key or the NUGET_TOKEN secret
#   PUBLISH_REQUIRED   output of preflight-nuget-release.mjs (default: true)
#   AUTH               trusted-publishing or api-key, used in the guidance
#   VERBOSE=1 or RUNNER_DEBUG=1 traces every command (off by default)
set -euo pipefail

if [ "${VERBOSE:-}" = 1 ] || [ "${RUNNER_DEBUG:-}" = 1 ]; then
  set -x
fi

package="${1:?Usage: push-nuget-package.sh <package.nupkg>}"
if [ "${PUBLISH_REQUIRED:-true}" != true ]; then
  echo "$(basename "$package") is already on nuget.org; nothing to publish."
  exit 0
fi
if [ ! -s "$package" ]; then
  echo "::error title=NuGet push failed::$package does not exist"
  exit 1
fi
if [ -z "${NUGET_API_KEY:-}" ]; then
  echo "::error title=NuGet push failed::No API key: configure the NUGET_USER variable for trusted publishing or the NUGET_TOKEN secret"
  exit 1
fi

log="$(mktemp)"
trap 'rm -f "$log"' EXIT
if dotnet nuget push "$package" \
  --source https://api.nuget.org/v3/index.json \
  --api-key "$NUGET_API_KEY" \
  --skip-duplicate 2>&1 | tee "$log"; then
  exit 0
fi

if grep -qE '(^|[^0-9])403([^0-9]|$)|Forbidden' "$log"; then
  if [ "${AUTH:-api-key}" = trusted-publishing ]; then
    echo "::error title=nuget.org rejected the trusted publishing key::Check that the nuget.org trusted publishing policy for this package names repository ${GITHUB_REPOSITORY:-<owner/repo>} and workflow file $(basename "${GITHUB_WORKFLOW_REF%@*}") and that its owner still owns the package."
  else
    echo "::error title=nuget.org rejected NUGET_TOKEN::The NUGET_TOKEN secret is invalid, expired, or not scoped to push this package. Create a new key at https://www.nuget.org/account/apikeys with push scope for the package and store it as the NUGET_TOKEN repository secret, or configure trusted publishing (https://learn.microsoft.com/nuget/nuget-org/trusted-publishing) and set the NUGET_USER repository variable."
  fi
fi
exit 1
