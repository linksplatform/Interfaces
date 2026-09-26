#!/usr/bin/env bash

set -euo pipefail

repository_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
solution="$repository_root/csharp/Platform.Interfaces.sln"
test_project="$repository_root/csharp/Platform.Interfaces.Tests/Platform.Interfaces.Tests.csproj"
package_project="$repository_root/csharp/Platform.Interfaces/Platform.Interfaces.csproj"
package_output=$(mktemp -d)

cleanup() {
  rm -rf "$package_output"
}
trap cleanup EXIT

dotnet restore "$solution" --nologo
dotnet build "$solution" --configuration Release --no-restore --no-incremental --nologo -warnaserror
dotnet test "$test_project" --configuration Release --framework net8 --no-build --no-restore --nologo
dotnet pack "$package_project" --configuration Release --no-build --no-restore \
  --output "$package_output" --nologo -warnaserror

mapfile -t packages < <(find "$package_output" -maxdepth 1 -type f -name '*.nupkg' -print)
mapfile -t symbol_packages < <(find "$package_output" -maxdepth 1 -type f -name '*.snupkg' -print)

if [[ ${#packages[@]} -ne 1 ]]; then
  echo "Expected one .nupkg, found ${#packages[@]}." >&2
  exit 1
fi

if [[ ${#symbol_packages[@]} -ne 0 ]]; then
  echo "Expected embedded symbols instead of a separate .snupkg." >&2
  exit 1
fi

package_contents=$(unzip -Z1 "${packages[0]}")
for expected_file in README.md icon.png lib/net8.0/Platform.Interfaces.dll; do
  if ! grep -Fxq "$expected_file" <<< "$package_contents"; then
    echo "${packages[0]} is missing $expected_file." >&2
    exit 1
  fi
done

echo "Validated ${packages[0]} with embedded symbols."
