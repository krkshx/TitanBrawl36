#!/bin/sh
set -e
cd "$(dirname "$0")"
cmake -B build -S . > /dev/null
cmake --build build -j"$(nproc)"
./build/linux/titan_pc .
