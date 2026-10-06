#!/usr/bin/env bash
# Consume the packed C++ NuGet package the way a project does, so a package
# that packs but cannot be used fails before it is published. Adapted from
# link-foundation/cpp-ai-driven-development-pipeline-template
# (scripts/check-nuget-package.sh): push it to a local folder feed, let
# `dotnet restore` extract it through <PackageDownload> (which skips the TFM
# check that a native package fails in an SDK-style project), check the MSBuild
# targets and headers, then compile a program against the restored headers.
#
# Usage: check-cpp-nuget-package.sh <package.nupkg>
# Environment:
#   CXX        C++20 compiler for the consumer program (default: c++)
#   VERBOSE=1  or RUNNER_DEBUG=1 traces every command (off by default)
set -euo pipefail

if [ "${VERBOSE:-}" = 1 ] || [ "${RUNNER_DEBUG:-}" = 1 ]; then
  set -x
fi

package="${1:?Usage: check-cpp-nuget-package.sh <package.nupkg>}"
package="$(cd "$(dirname "$package")" && pwd)/$(basename "$package")"
work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT

# <id>.<version>.nupkg; the version is the last three dot-separated fields.
name="$(basename "$package" .nupkg)"
version="$(printf '%s\n' "$name" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+([-+].*)?$')"
id="${name%."$version"}"

mkdir -p "$work/feed" "$work/project"
dotnet nuget push "$package" --source "$work/feed"
cat > "$work/project/check.csproj" <<XML
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup><TargetFramework>netstandard2.0</TargetFramework><DisableImplicitNuGetFallbackFolder>true</DisableImplicitNuGetFallbackFolder><DisableImplicitFrameworkReferences>true</DisableImplicitFrameworkReferences></PropertyGroup>
  <ItemGroup><PackageDownload Include="$id" Version="[$version]" /></ItemGroup>
</Project>
XML
cat > "$work/project/nuget.config" <<XML
<configuration><packageSources><clear /><add key="local" value="$work/feed" /></packageSources></configuration>
XML
dotnet restore "$work/project/check.csproj" --packages "$work/packages"

extracted="$work/packages/$(printf '%s' "$id" | tr '[:upper:]' '[:lower:]')/$version"
include="$extracted/lib/native/include"
if [ ! -f "$extracted/build/native/$id.targets" ]; then
  echo "::error title=C++ NuGet package unusable::build/native/$id.targets is missing"
  exit 1
fi
if ! grep -q 'lib\\native\\include' "$extracted/build/native/$id.targets"; then
  echo "::error title=C++ NuGet package unusable::build/native/$id.targets does not add lib/native/include to the include path"
  exit 1
fi

# The nuspec tells users to include Platform.Interfaces.h only, so it has to
# pull in every header it needs from the package.
cat > "$work/consumer.cpp" <<'CPP'
#include <Platform.Interfaces.h>
#include <vector>

static_assert(Platform::Interfaces::CList<std::vector<int>>);

int main() { return 0; }
CPP
if ! "${CXX:-c++}" -std=c++20 -I "$include" -o "$work/consumer" "$work/consumer.cpp"; then
  echo "::error title=C++ NuGet package unusable::A program that includes Platform.Interfaces.h from the restored package does not compile"
  exit 1
fi
"$work/consumer"
echo "NuGet restore and use of $id $version: OK"
