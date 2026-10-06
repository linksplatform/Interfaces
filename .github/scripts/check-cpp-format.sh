#!/usr/bin/env bash
# Check that every tracked C++ source matches .clang-format, as the
# link-foundation/cpp-ai-driven-development-pipeline-template lint step does
# (`clang-format --dry-run --Werror`). .clang-format links to the Settings
# submodule, so that submodule has to be checked out first.
#
# Environment:
#   CLANG_FORMAT  clang-format executable (default: clang-format); CI pins the
#                 PyPI wheel so local runs can use exactly the same version
#   VERBOSE=1     or RUNNER_DEBUG=1 traces every command (off by default)
# Fix the reported files with: git ls-files -z -- 'cpp/*.h' 'cpp/*.cpp' | xargs -0 clang-format -i
set -euo pipefail

if [ "${VERBOSE:-}" = 1 ] || [ "${RUNNER_DEBUG:-}" = 1 ]; then
  set -x
fi

cd "$(dirname "$0")/../.."
clang_format="${CLANG_FORMAT:-clang-format}"

if [ ! -f .clang-format ]; then
  echo "::error title=clang-format style missing::.clang-format does not resolve; run: git submodule update --init Settings" >&2
  exit 1
fi
"$clang_format" --version

# -z keeps file names such as "ICounter[TResult, TArgument].h" intact.
count="$(git ls-files -z -- 'cpp/*.h' 'cpp/*.cpp' | tr -cd '\0' | wc -c)"
if [ "$count" -eq 0 ]; then
  echo "::error title=clang-format checked nothing::no C++ files are tracked under cpp/" >&2
  exit 1
fi
if ! git ls-files -z -- 'cpp/*.h' 'cpp/*.cpp' | xargs -0 "$clang_format" --dry-run --Werror; then
  echo "::error title=C++ formatting::run clang-format -i on the files above (see $0)" >&2
  exit 1
fi
echo "clang-format: $count C++ files are formatted"
