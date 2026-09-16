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
  --include-symbols --output "$package_output" --nologo -warnaserror

mapfile -t packages < <(find "$package_output" -maxdepth 1 -type f -name '*.nupkg' -print)
mapfile -t symbol_packages < <(find "$package_output" -maxdepth 1 -type f -name '*.snupkg' -print)

if [[ ${#packages[@]} -ne 1 ]]; then
  echo "Expected one .nupkg, found ${#packages[@]}." >&2
  exit 1
fi

if [[ ${#symbol_packages[@]} -ne 1 ]]; then
  echo "Expected one .snupkg, found ${#symbol_packages[@]}." >&2
  exit 1
fi

for expected_file in README.md icon.png lib/net8.0/Platform.Interfaces.dll; do
  if ! unzip -Z1 "${packages[0]}" | grep -Fxq "$expected_file"; then
    echo "${packages[0]} is missing $expected_file." >&2
    exit 1
  fi
done

if ! unzip -Z1 "${symbol_packages[0]}" | grep -Fxq 'lib/net8.0/Platform.Interfaces.pdb'; then
  echo "${symbol_packages[0]} is missing lib/net8.0/Platform.Interfaces.pdb." >&2
  exit 1
fi

echo "Validated ${packages[0]} and ${symbol_packages[0]}."
