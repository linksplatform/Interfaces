#!/usr/bin/env bash
# Reproduce issue #150's C++ false positive: the shared Workflows cpp-test.yml
# only ran `cmake --build .`, so a failing GoogleTest still produced a green
# run. This copies cpp/ to a temporary folder, adds a test that always fails,
# and shows that the old command passes while cpp/build-and-test.sh fails.
# Requires conan (2.x), cmake and a C++20 compiler on PATH.
set -euo pipefail

repository_root="$(cd "$(dirname "$0")/.." && pwd)"
work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT
cp -r "$repository_root/cpp" "$work/cpp"
rm -rf "$work/cpp/build"
cat >> "$work/cpp/Platform.Interfaces.Tests/Platform.Interfaces.Tests.cpp" <<'CPP'

TEST(Issue150, AlwaysFails) { FAIL() << "This test must make CI red"; }
CPP

echo "== Old shared workflow command (build only)"
(
  cd "$work/cpp"
  conan install . --output-folder=old --build=missing --settings:all compiler.cppstd=20 >/dev/null
  cmake -S . -B old -DCMAKE_TOOLCHAIN_FILE=old/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release >/dev/null
  cmake --build old >/dev/null
) && echo "old command: PASSED (false positive)"

echo "== New cpp/build-and-test.sh"
if "$work/cpp/build-and-test.sh" >"$work/new.log" 2>&1; then
  echo "new script: PASSED (unexpected)"
  exit 1
fi
grep -E "AlwaysFails|tests passed" "$work/new.log" || true
echo "new script: FAILED as expected"
