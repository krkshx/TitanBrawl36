# Third-party code inside libg.so — NOT reimplemented

Of 37,064 functions in the IDA inventory (`data/ida_shard_*.csv`), ~9.9k
belong to vendored third-party libraries. Reimplementing them would be
wrong: on every target platform we link the canonical upstream instead.
This file maps binary symbols to the replacement.

| Symbols in binary | Upstream | Replacement on our targets |
|---|---|---|
| `St6__ndk*`, `_Znwm`, `_ZdlPv`, `__cxa_*`, `emutls_*`, `Unwind_*` | libc++ (NDK) | system STL (`std::`) |
| `icu_58*`, `u_*_58`, `udata_*`, `unorm*`, `UCNV_*` | ICU 58 | system ICU / Win32 NLS |
| `hb_*`, `OT::*`, graphite bits | HarfBuzz | system HarfBuzz |
| `_png_*`, `png_*` | libpng | system libpng / stb_image |
| `_FT_*`, `FT_*`, `ft_*`, `TT_*`, `cff_*`, `sfnt_*`, `af_*`, `cf2_*`, `FTC_*` | FreeType | system FreeType |
| `_ZSTD*`, `ZSTD*`, `_HUF*`, `HUF_*`, `_FSE*`, `_HIST*`, `_POOL*`, `_XXH*`, `ZDICT_*`, `ZSTDMT_*` | zstd | system libzstd |
| `lzham*` (game-candidate list) | lzham | vendor lzham (see below) |
| `asio*` | standalone Asio | standalone Asio (submodule) |
| `FMOD*` | FMOD (proprietary) | FMOD SDK / miniaudio shim |
| `gl*`, `egl*` | OpenGL ES (Android) | GLES3 / ANGLE / desktop GL |
| `AAsset*`, `ANative*`, `Java_*` | Android NDK / JVM | platform layer (`src/platform/`) |
| `safe_jni*`, `yoozoo*`, `miit*`, `linecorp*`, `nova*` | CN/JP SDK bridges | platform layer stubs |
| `sentry_*` | sentry-native | sentry-native / off |
| `titan3com*supercell5titan*` | `com.supercell.titan` JNI | platform layer |

`lzham`/`sc`/`Databind`/`wendelstein` look vendored-but-game-specific and
stay in scope for reimplementation (they are Supercell's own `sc` engine).

Rule: anything in this table is marked `ThirdPartyExternal` in the
generated function registry and must never get a hand-written body.
