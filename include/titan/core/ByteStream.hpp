#pragma once

#include "titan/core/ChecksumEncoder.hpp"
#include <cstddef>
#include <string>
#include <vector>

// ByteStream — reversed from libg_decrypted.so (ARM64).
//
// Layout in the binary (offsets from `this`):
//   +12 checksum state (see ChecksumEncoder)
//   +20 length/offset (i32)
//   +28 bitOffset for writeBoolean packing (i32, 0..7)
//   +32 buffer (u8*)
//   +40 capacity (i32)
// Growth policy observed: allocate capacity+101 (boolean/byte),
// +104 (int/bytes), +105 (vint); string/bytes grow by len+capacity+104.
//
// Wire format (must stay bit-exact with the client):
// - writeInt: 4 bytes big-endian.
// - writeBoolean: bit-packed into the current byte (bitOffset 0..7).
// - writeVInt: 1..5 byte varint, sign-aware (see .cpp).
// - writeString: writeInt(byteLength) + raw bytes; null -> writeInt(-1);
//   strings >= 900001 bytes are rejected and encoded as -1 (see @0x5174d0).
// - writeBytes(data,len): writeInt(len) + raw bytes; null -> writeInt(-1).

namespace titan {

class ByteStream : public ChecksumEncoder {
public:
    explicit ByteStream(std::size_t initialCapacity = 128);
    ~ByteStream() override;

    ByteStream(const ByteStream&) = delete;
    ByteStream& operator=(const ByteStream&) = delete;

    // --- writers (IDA refs in .cpp) ---
    void writeBoolean(bool value);            // @0x3294c0
    void writeInt(i32 value) override;        // @0x3f3bd0
    void writeVInt(i32 value) override;       // @0x298f64
    void writeString(const std::string* value); // @0x5174d0 (null -> -1)
    void writeBytes(const u8* data, i32 len); // @0x61bd08 (null -> -1)
    void writeByte(i8 value);
    void writeShort(i16 value);
    void writeLongLong(i64 value);

    // --- buffer access ---
    [[nodiscard]] const u8* data() const { return buffer_.data(); }
    [[nodiscard]] i32 size() const { return length_; }
    [[nodiscard]] i32 bitOffset() const { return bitOffset_; }
    void clear();

private:
    void ensureCapacity(i32 extra);
    void writeIntToByteArray(i32 value); // @0x87bc64

    std::vector<u8> buffer_;
    i32 length_ = 0;    // +20
    i32 bitOffset_ = 0; // +28
};

} // namespace titan
