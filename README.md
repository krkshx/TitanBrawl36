# TitanBrawl36

C++17 reimplementation of Brawl Stars v36 (`libg_decrypted.so`, ARM64
Android), reversed function-by-function through IDA Pro. Cross-platform
(Linux / Windows / macOS / Android NDK), CMake, no mandatory dependencies.

## Layout

- `titan/` — the library (`titan_core`), single tree, no `include/`+`src/`
  split, CPP-only (no headers anywhere): `core/` (streams, message base),
  `messages/<family>/` (335 protocol classes, one `.cpp` per class),
  `commands/<family>/` (59, one `.cpp` per class + generated
  `AllCommands.cpp`), `game/<family>/` (logic + data classes, one `.cpp`
  per class), `crypto/` (session crypto, libsodium optional), `sc/`
  (engine), `net/` (framing), `gen/` (generated factory + registry,
  also CPP-only).
- `titan/messages/pending/` — only not-yet-reversed stubs and
  documented aliases. No forwarders (deleted on reversal).
- `tests/` — 13 self-check suites, all must stay green.
- `tools/` — living generators (`generate_registry`, `gen_dataclasses`,
  `gen_notifications`, …); one-shot migrations rest in `tools/archive/`.
- `viewer/` — Qt home-data viewer (optional, `-DTITAN_BUILD_VIEWER=ON`).
- `docs/` — `IDA_BASELINE.md` (binary source of truth), `PROTOCOL.md`
  (wire contract), `REVERSE_STATUS.md` (progress + layout rule),
  `THIRDPARTY.md`, `tutor.md`/`tutor64.md` (reversing guides).
- `data/` — IDA inventory shards, registry inputs, table maps.
- `assets/` — local-only decompiled game data (git-ignored, needed by
  the datatables test via `./assets` or `TITAN_ASSETS`).

## Build

```sh
cmake -B build -S . -DTITAN_BUILD_TESTS=ON
cmake --build build -j$(nproc)
ctest --test-dir build
cmake -B build -S . -DTITAN_WITH_SODIUM=ON  # real crypto vectors
```

## Naming

Class/method names come from the binary's symbols; field names do NOT
exist in the binary (verified: IDA holds no game struct layouts), so
fields carry binary offsets (`v144_` = `*(this+144)`) and gain semantic
names once a getter proves them (`getDiamonds`). Every function cites
its IDA address.
