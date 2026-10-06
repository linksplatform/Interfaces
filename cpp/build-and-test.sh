#!/usr/bin/env bash
# Build the C++ tests with Conan 2 and CMake, then run them through CTest.
# CI and Gitpod both call this script so a passing build always means the
# tests were executed (issue #150: the shared workflow only compiled them).
#
# Environment:
#   BUILD_DIR   build directory relative to cpp/ (default: build)
#   BUILD_TYPE  CMake/Conan build type (default: Release)
#   SANITIZERS  sanitizers for the tests, e.g. "address;undefined" (default: none)
#   CC, CXX     compilers; `conan profile detect` and CMake both read them
#   VERBOSE=1   or RUNNER_DEBUG=1 traces every command (off by default)
set -euo pipefail

if [ "${VERBOSE:-}" = 1 ] || [ "${RUNNER_DEBUG:-}" = 1 ]; then
  set -x
fi

cd "$(dirname "$0")"
build_dir="${BUILD_DIR:-build}"
build_type="${BUILD_TYPE:-Release}"

if ! conan profile path default >/dev/null 2>&1; then
  conan profile detect
fi

conan install . --output-folder="$build_dir" --build=missing \
  --settings:all build_type="$build_type" --settings:all compiler.cppstd=20
cmake -S . -B "$build_dir" \
  -DCMAKE_TOOLCHAIN_FILE="$build_dir/conan_toolchain.cmake" \
  -DCMAKE_BUILD_TYPE="$build_type" \
  -DLINKS_PLATFORM_TESTS=ON \
  -DLINKS_PLATFORM_SANITIZERS="${SANITIZERS:-}"
# --config and -C select the build type of multi-config generators (MSVC).
cmake --build "$build_dir" --config "$build_type" --parallel
# --no-tests=error turns "nothing was run" into a failure instead of a pass.
ctest --test-dir "$build_dir" --build-config "$build_type" --output-on-failure --no-tests=error
