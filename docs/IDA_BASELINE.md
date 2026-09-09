# IDA baseline — libg_decrypted.so (v36, ARM64)

Source of truth for the reimplementation. Verified live via IDA MCP.

## Binary

- File: `libg_decrypted.so` (Brawl Stars v36, ARM64 Android)
- IDA path: `/home/sahar/scd/libg_decrypted.so`
- base: `0x0`, size: `0x1216838`, filesize: `0x31e844c`
- md5: `df4b493633a68f6a2abf83bfcf585912`
- sha256: `b92b8158adbd0223a73e0846ccf31c2fac65cf42efe2e2e1275ae9307c70b02b`
- crc32: `0x62d1ce20`
- Functions: ~37,500 (enumerated via `list_functions`; offsets 0..~37500
  return data, 38000+ empty; tail contains unnamed thunks)
- Debug symbols: present (mangled DWARF names, demangled by IDA).
  No fixup script needed on ARM64 (unlike ARM32 tutor).

## Method (tutor64.md, verified)

1. Message ID: Search immediate value, filter `MOV W0, #imm` + `RET`
   (getter `::getMessageType`).
2. xref (data) from getter into vtable (8-byte slots):
   `0:D2 1:D0 2:encode 3:decode 4:getServiceNodeType 5:getMessageType
   6:getMessageTypeName 7:destruct`.
3. `encode` uses write-methods, `decode` uses read-methods. Prefer `encode`.
4. `ByteStream` embedded at `this+16` (was +8 on ARM32),
   message fields e.g. `this+132` (was +80 on ARM32).
5. Indirect calls `(*(*a2 + offLL))(a2, ...)` decode via ByteStream vtable:

| off hex | off dec (Hex-Rays) | fn |
|---|---|---|
| 0x38 | 56 | writeString |
| 0x40 | 64 | writeBoolean |
| 0x48 | 72 | writeInt |
| 0x50 | 80 | writeInt8 |
| 0x58 | 88 | writeInt16 |
| 0x60 | 96 | writeInt24 |
| 0x68 | 104 | writeBytes |
| 0x70 | 112 | writeByte |
| 0x78 | 120 | writeShort |
| 0x80 | 128 | writeVInt |
| 0x88 | 136 | writeVLong |
| 0x90 | 144 | writeLongLong |
| 0x98 | 152 | writeLong(LogicLong) |
| 0xA8 | 168 | readStringReference |
| 0xB0 | 176 | readString |
| 0xD0 | 208 | readBoolean |
| 0xD8 | 216 | readInt |
| 0xF8 | 248 | readVInt |

ByteStream vtable base in this IDB: `0xd5c468`.

## Verified examples

- `ResetAccountMessage::getMessageType` = `0x6cdf20` → `MOV W0, #0x2784; RET` (10116)
- `ResetAccountMessage::encode` = `0x4465cc`:
  `PiranhaMessage::encode(a1); ByteStream::writeInt(a1+16, *(a1+132))`
- `ByteStream::writeVInt` = `0x298f64` (checksum update + 1..5 byte varint)
- `ByteStream::writeInt` = `0x3f3bd0` = `ChecksumEncoder::writeInt + writeIntToByteArray`
- `ByteStream::writeIntToByteArray` = `0x87bc64` (big-endian, grow +104)
- `ChecksumEncoder::writeVInt` = `0x5c50e0`: `state12 = v + (state12>>31) + 33`
- `ChecksumEncoder::writeInt` = `0x8a558c`: `state12 = v + (state12>>31) + 9`
- `PiranhaMessage::encode` = `0x8f2e70`: anti-tamper return-address guard,
  else no-op (calls `AntiCheat::guard_callback(0x1F)` on violation)

## Object layout (ARM64, from decompiles)

ByteStream (+0 = ChecksumEncoder base):
- +12: checksum state (i32)
- +20: length/offset (i32)
- +28: bitOffset for writeBoolean (i32, 0..7)
- +32: buffer (u8*)
- +40: capacity (i32)
- growth steps observed: +101 (boolean/byte), +104 (int), +105 (vint)

Every reimplemented function cites its IDA address in a comment.
