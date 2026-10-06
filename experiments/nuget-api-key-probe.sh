#!/usr/bin/env bash
# Shows that nuget.org validates an API key for one package without publishing
# anything: POST /api/v2/package/create-verification-key/<id> answers 200 for a
# key that may push the package and 401/403 otherwise, with the same message
# that the failing `dotnet nuget push` printed in issue #150. With a valid key
# the response carries a one-time key; .github/scripts/preflight-nuget-release.mjs
# then calls GET /api/v2/verifykey/<id>/<version> with it to check the package
# scope (see NuGetGallery ApiController.CreatePackageVerificationKeyAsync).
#
# Usage: experiments/nuget-api-key-probe.sh [package-id ...]
# Environment: NUGET_API_KEY (default: an obviously invalid key)
set -euo pipefail
key="${NUGET_API_KEY:-00000000-invalid-key-for-the-probe}"
[ "$#" -gt 0 ] || set -- Platform.Interfaces Platform.Interfaces.TemplateLibrary
for id in "$@"; do
  for header in with-key without-key; do
    args=(--silent --show-error --max-time 30 --request POST --data '' --write-out '\nHTTP %{http_code}\n')
    [ "$header" = with-key ] && args+=(--header "X-NuGet-ApiKey: $key")
    echo "== $id ($header)"
    curl "${args[@]}" "https://www.nuget.org/api/v2/package/create-verification-key/$id" || true
  done
done
