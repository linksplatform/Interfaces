#!/bin/bash
set -euo pipefail

sudo apt-get update
sudo apt-get install -y g++ cmake pipx

pipx install conan
pipx ensurepath
export PATH="$HOME/.local/bin:$PATH"
conan profile detect --force
