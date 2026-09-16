#!/bin/sh
# TitanBrawl36 — сборка под Linux.
# Выход: build/linux/titan_pc + build/linux/libg.so (запуск сам).
# Использование: ./build.sh [clean] [debug] [-jN]
set -e
cd "$(dirname "$0")"

TYPE=Release
if command -v nproc >/dev/null 2>&1; then JOBS=$(nproc); else JOBS=4; fi

for a in "$@"; do
  case "$a" in
    clean) echo "[build] clean..."; rm -rf build ;;
    debug) TYPE=Debug ;;
    -j*) JOBS=${a#-j} ;;
  esac
done

echo "[build] configure ($TYPE)..."
cmake -B build -S . -DCMAKE_BUILD_TYPE=$TYPE > /dev/null
echo "[build] compile (jobs=$JOBS)..."
cmake --build build -j"$JOBS"
echo "[build] OK: build/linux/titan_pc + build/linux/libg.so"
