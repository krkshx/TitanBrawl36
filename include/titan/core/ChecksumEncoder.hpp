#pragma once

#include "titan/core/Types.hpp"

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
//     writeVInt    -> K = 33                      @0x5c50e0
//     writeInt     -> K = 9                       @0x8a558c

namespace titan {

class ChecksumEncoder {
public:
    ChecksumEncoder();
    virtual ~ChecksumEncoder();

    virtual void writeBoolean(bool value);   // @0x534690
    virtual void writeVInt(i32 value);       // @0x5c50e0
    virtual void writeInt(i32 value);        // @0x8a558c
    virtual void writeStringLength(i32 charLength, bool isNull); // @0x6ed97c shape

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
