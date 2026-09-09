#pragma once

#include "titan/core/ChecksumEncoder.hpp"
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

// ByteStream — reversed from libg_decrypted.so (ARM64).
//
// Layout in the binary (offsets from `this`):
//   +12 checksum state (see ChecksumEncoder)
//   +20 length/offset (i32) — write cursor and read cursor alike
//   +28 bitOffset for boolean bit-packing (i32, 0..7)
//   +32 buffer (u8*)
//   +40 capacity (i32)
// Growth policy observed: allocate capacity+101 (boolean/byte),
// +104 (int/bytes), +105 (vint); string/bytes grow by len+capacity+104.
//
// Wire format (must stay bit-exact with the client):
// - Int: 4 bytes big-endian.
// - Boolean: bit-packed into the current byte (bitOffset 0..7).
// - VInt: 1..5 byte varint, sign-aware (see .cpp).
// - String: writeInt(byteLength) + raw bytes; null -> writeInt(-1);
//   strings >= 900001 bytes are rejected and encoded as -1 (see @0x5174d0).
// - Bytes(data,len): writeInt(len) + raw bytes; null -> writeInt(-1).
//
// Reader mirrors the writer and resets bitOffset the same way:
// readInt @0x191dc4, readVInt @0x356c40, readBoolean @0x23bc10.

namespace titan {

class ByteStream : public ChecksumEncoder {
public:
    explicit ByteStream(std::size_t initialCapacity = 128);
    ~ByteStream() override;

    ByteStream(const ByteStream&) = delete;
    ByteStream& operator=(const ByteStream&) = delete;

    // --- writers (IDA refs in .cpp) ---
    void writeBoolean(bool value);              // @0x3294c0
    void writeInt(i32 value) override;          // @0x3f3bd0
    void writeVInt(i32 value) override;         // @0x298f64
    void writeString(const std::string* value); // @0x5174d0 (null -> -1)
    void writeBytes(const u8* data, i32 len);   // @0x61bd08 (null -> -1)
    void writeByte(i8 value);
    void writeShort(i16 value);
    void writeLongLong(i64 value);

    // --- readers ---
    bool readBoolean();                    // @0x23bc10
    i32 readInt();                         // @0x191dc4
    i32 readVInt();                        // @0x356c40
    std::optional<std::string> readString();
    std::vector<u8> readBytes();

    // Load a received buffer for decoding (sets cursor to 0).
    void setBuffer(const u8* data, i32 len);

    // --- buffer access ---
    [[nodiscard]] const u8* data() const { return buffer_.data(); }
    [[nodiscard]] i32 size() const { return length_; }
    [[nodiscard]] i32 cursor() const { return readCursor_; }
    [[nodiscard]] i32 bitOffset() const { return bitOffset_; }
    void clear();

private:
    void ensureCapacity(i32 extra);
    void putByte(u8 b);
    void writeIntToByteArray(i32 value); // @0x87bc64
    [[nodiscard]] u8 readByteRaw();

    std::vector<u8> buffer_;
    i32 length_ = 0;     // +20 (write cursor)
    i32 bitOffset_ = 0;  // +28
    i32 readCursor_ = 0; // read cursor (binary reuses +20 after reset)
};

} // namespace titan
