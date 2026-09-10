#pragma once

// BitStream — bit-level stream, reversed from libg_decrypted.so (ARM64).
//
// Used by ClientInputMessage (10555) and VisionUpdateMessage (24109)
// payloads. LSB-first within each byte, sequential across bytes.
//
// Layout in the binary:
//   +8  byteIndex/cursor (i32), +12 length2 (i32), +16 bitOffset (0..7),
//   +24 buffer (u8*), +32 capacity (i32), +36 ownsBuffer (u8).
// getLength = max(byteIndex+1, length2).
//
// writeInt(v, bits): clamp to [-2^bits+1, 2^bits-1]; sign bit first
// (1 = v>=0), then magnitude LSB-first. Total 1+bits on the wire.
// writePositiveInt(v, bits): clamp to [0, 2^bits-1]; bits LSB-first.
// writePositiveVInt(v, prefixBits): bit-length of v in prefixBits, then v.
// Out-of-range values log an error in the binary; we clamp identically.
//
// Addresses: writeInt @0x53bd24, writePositiveInt @0x8d18d0,
// writeBoolean @0x23ca7c, readBoolean @0x54dc78,
// writePositiveVInt @0x53c590, ensureCapacity @0x624e28,
// ctor @0x7b69dc, vtable @0xd7e940.

#include "titan/core/Types.hpp"

#include <cstddef>
#include <vector>

namespace titan {

class BitStream {
public:
    explicit BitStream(i32 capacity = 64); // @0x7b69dc
    ~BitStream();                          // @0x9cb464/@0xa206f0

    BitStream(const BitStream&) = delete;
    BitStream& operator=(const BitStream&) = delete;

    void writeBoolean(bool v);              // @0x23ca7c (= writePositiveInt(v&1,1))
    void writeInt(i32 v, int bits);         // @0x53bd24
    void writePositiveInt(i32 v, int bits); // @0x8d18d0
    void writePositiveVInt(i32 v, int prefixBits); // @0x53c590

    bool readBoolean();              // @0x54dc78
    i32 readPositiveInt(int bits);
    i32 readPositiveVInt(int prefixBits);

    [[nodiscard]] const u8* getByteArray() const { return buffer_.data(); } // @0x6c8e2c
    [[nodiscard]] i32 getLength() const; // @0x5f56ac: max(byteIndex+1, length2)
    [[nodiscard]] i32 getOffset() const { return byteIndex_; }

    void setBuffer(const u8* data, i32 len); // read path setup

private:
    void ensureCapacity(); // @0x624e28
    void writeBit(bool b);
    bool readBit();

    std::vector<u8> buffer_;
    i32 byteIndex_ = 0; // +8
    i32 length2_ = 0;   // +12
    i32 bitOffset_ = 0; // +16
};

} // namespace titan
