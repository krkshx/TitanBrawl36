// ChecksumEncoder — reversed from libg_decrypted.so (ARM64).
// Addresses refer to the IDA baseline in docs/IDA_BASELINE.md.

#include "titan/core/ChecksumEncoder.hpp"

namespace titan {

ChecksumEncoder::ChecksumEncoder() = default;
ChecksumEncoder::~ChecksumEncoder() = default;

// @0x534690 — ChecksumEncoder::writeBoolean
//   v2 = 13 if value else 7; state = rotl(state,1) + v2
void ChecksumEncoder::writeBoolean(bool value) {
    const u32 k = value ? 13u : 7u;
    checksum_ = rotated() + k;
}

// @0x5c50e0 — ChecksumEncoder::writeVInt
//   state = value + rotl(state,1) + 33
void ChecksumEncoder::writeVInt(i32 value) {
    checksum_ = static_cast<u32>(value) + rotated() + 33u;
}

// @0x8a558c — ChecksumEncoder::writeInt
//   state = value + rotl(state,1) + 9
void ChecksumEncoder::writeInt(i32 value) {
    checksum_ = static_cast<u32>(value) + rotated() + 9u;
}

// @0x739a8c — ChecksumEncoder::writeVLong
// Chained 64-bit fold over the (high, low) halves:
//   v7 = (u64)state<<32 | state;  x = high + (u32)(v7>>31) + 65;
//   v7b = (u64)x<<32 | x;         state = low + (u32)(v7b>>31) + 88.
// (The >>31 terms are the binary's rotl idiom evaluated in 64 bits,
// hence the extra carried bit vs rotated(); reproduced exactly.)
void ChecksumEncoder::writeVLong(i64 value) {
    const u64 u = static_cast<u64>(value);
    const u32 lo = static_cast<u32>(u);
    const u32 hi = static_cast<u32>(u >> 32);
    const u64 v7 = (static_cast<u64>(checksum_) << 32) | checksum_;
    const u32 x = hi + static_cast<u32>(v7 >> 31) + 65u;
    const u64 v7b = (static_cast<u64>(x) << 32) | x;
    checksum_ = lo + static_cast<u32>(v7b >> 31) + 88u;
}

// @0x6ed97c — ChecksumEncoder::writeString shape
//   state = rotl(state,1) + charLength + 28 (null -> rotl + 27)
void ChecksumEncoder::writeStringLength(i32 charLength, bool isNull) {
    checksum_ = isNull ? rotated() + 27u
                       : rotated() + static_cast<u32>(charLength) + 28u;
}

// @0x69a564 — ChecksumEncoder::writeStringReference
//   state = rotl(state,1) + charLength + 38 (reference is never null)
void ChecksumEncoder::writeStringReferenceLength(i32 charLength) {
    checksum_ = rotated() + static_cast<u32>(charLength) + 38u;
}

// @0x7c317c — ChecksumEncoder::writeBytes
//   state = rotl(state,1) + len + 38 (null -> rotl + 37)
void ChecksumEncoder::writeBytesLength(i32 len, bool isNull) {
    checksum_ = isNull ? rotated() + 37u
                       : rotated() + static_cast<u32>(len) + 38u;
}

} // namespace titan
