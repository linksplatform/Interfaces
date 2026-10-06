#!/usr/bin/env bash
# Shows that the sanitizer job in cpp-test.yml is not a false negative: a test
# with a heap overflow and a signed overflow passes a plain build but fails
# with SANITIZERS="address;undefined". Copies cpp/ to a temporary folder.
# Requires conan (2.x), cmake and gcc or clang on PATH.
set -euo pipefail

repository_root="$(cd "$(dirname "$0")/.." && pwd)"
work="$(mktemp -d)"
trap '[ -n "${KEEP:-}" ] || rm -rf "$work"' EXIT
cp -r "$repository_root/cpp" "$work/cpp"
find "$work/cpp" -maxdepth 1 -type d -name "build*" -exec rm -rf {} +
cat >> "$work/cpp/Platform.Interfaces.Tests/Platform.Interfaces.Tests.cpp" <<'CPP'

#include <climits>
TEST(Sanitizers, HeapOverflow) {
    volatile int index = 4;
    int* values = new int[4]{};
    int value = values[index];
    delete[] values;
    EXPECT_GE(value, INT_MIN);
}
TEST(Sanitizers, SignedOverflow) {
    volatile int big = INT_MAX;
    EXPECT_NE(big + 1, 0);
}
CPP

for build in plain sanitizers; do
  sanitizers=""
  [ "$build" = sanitizers ] && sanitizers="address;undefined"
  echo "== SANITIZERS=\"$sanitizers\""
  if BUILD_DIR="build-$build" BUILD_TYPE=Debug SANITIZERS="$sanitizers" \
      "$work/cpp/build-and-test.sh" >"$work/run-$build.log" 2>&1; then
    echo "PASSED"
  else
    grep -E "ERROR: AddressSanitizer|runtime error|tests passed" "$work/run-$build.log" || true
    echo "FAILED"
  fi
done
