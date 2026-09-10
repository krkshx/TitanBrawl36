#pragma once

#include "titan/core/Types.cpp"

// ChecksumEncoder — base of ByteStream in libg.so.
//
// IDA notes (ARM64, libg_decrypted.so):
// - Checksum state lives at this+12 (i32).
// - Every write* first folds into the checksum with the same shape:
//     state = payloadTerm + rotl32(state, 1) + K
//   where the decompiler renders rotl32(state,1) as `(v >> 31)` with
//   `v = (u64)state<<32 | state` (see docs/IDA_BASELINE.md).
// - Constants observed:
//     writeBoolean -> K = 13 (true) / 7 (false)   @0x534690
//     writeString  -> K = 28 + charLength, 27 if null @0x6ed97c
//     writeStringReference -> K = 38 + charLength @0x69a564
//     writeBytes   -> K = 38 + len, 37 if null    @0x7c317c
//     writeVInt    -> K = 33                      @0x5c50e0
//     writeInt     -> K = 9                       @0x8a558c
//     writeVLong   -> chained fold (see .cpp)     @0x739a8c
// (charLength = UTF-16 units per getCharLength @0x62b658.)

namespace titan {

class ChecksumEncoder {
public:
    ChecksumEncoder();
    virtual ~ChecksumEncoder();

    virtual void writeBoolean(bool value);   // @0x534690
    virtual void writeVInt(i32 value);       // @0x5c50e0
    virtual void writeInt(i32 value);        // @0x8a558c
    virtual void writeVLong(i64 value);      // @0x739a8c
    virtual void writeStringLength(i32 charLength, bool isNull); // @0x6ed97c shape
    virtual void writeStringReferenceLength(i32 charLength);     // @0x69a564
    virtual void writeBytesLength(i32 len, bool isNull);         // @0x7c317c

    [[nodiscard]] u32 checksum() const { return checksum_; }
    void setChecksum(u32 value) { checksum_ = value; }

protected:
    // rotl32(state,1) as the binary computes it (see header comment).
    [[nodiscard]] u32 rotated() const {
        return static_cast<u32>((checksum_ << 1) | (checksum_ >> 31));
    }

    u32 checksum_ = 0; // == *(this+12) in the binary
};

} // namespace titan

// ChecksumEncoder — reversed from libg_decrypted.so (ARM64).
// Addresses refer to the IDA baseline in docs/IDA_BASELINE.md.


namespace titan {

inline ChecksumEncoder::ChecksumEncoder() = default;
inline ChecksumEncoder::~ChecksumEncoder() = default;

// @0x534690 — ChecksumEncoder::writeBoolean
//   v2 = 13 if value else 7; state = rotl(state,1) + v2
inline void ChecksumEncoder::writeBoolean(bool value) {
    const u32 k = value ? 13u : 7u;
    checksum_ = rotated() + k;
}

// @0x5c50e0 — ChecksumEncoder::writeVInt
//   state = value + rotl(state,1) + 33
inline void ChecksumEncoder::writeVInt(i32 value) {
    checksum_ = static_cast<u32>(value) + rotated() + 33u;
}

// @0x8a558c — ChecksumEncoder::writeInt
//   state = value + rotl(state,1) + 9
inline void ChecksumEncoder::writeInt(i32 value) {
    checksum_ = static_cast<u32>(value) + rotated() + 9u;
}

// @0x739a8c — ChecksumEncoder::writeVLong
// Chained 64-bit fold over the (high, low) halves:
//   v7 = (u64)state<<32 | state;  x = high + (u32)(v7>>31) + 65;
//   v7b = (u64)x<<32 | x;         state = low + (u32)(v7b>>31) + 88.
// (The >>31 terms are the binary's rotl idiom evaluated in 64 bits,
// hence the extra carried bit vs rotated(); reproduced exactly.)
inline void ChecksumEncoder::writeVLong(i64 value) {
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
inline void ChecksumEncoder::writeStringLength(i32 charLength, bool isNull) {
    checksum_ = isNull ? rotated() + 27u
                       : rotated() + static_cast<u32>(charLength) + 28u;
}

// @0x69a564 — ChecksumEncoder::writeStringReference
//   state = rotl(state,1) + charLength + 38 (reference is never null)
inline void ChecksumEncoder::writeStringReferenceLength(i32 charLength) {
    checksum_ = rotated() + static_cast<u32>(charLength) + 38u;
}

// @0x7c317c — ChecksumEncoder::writeBytes
//   state = rotl(state,1) + len + 38 (null -> rotl + 37)
inline void ChecksumEncoder::writeBytesLength(i32 len, bool isNull) {
    checksum_ = isNull ? rotated() + 37u
                       : rotated() + static_cast<u32>(len) + 38u;
}

} // namespace titan

