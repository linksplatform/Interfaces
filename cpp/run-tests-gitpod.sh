#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"
./build-and-test.sh
