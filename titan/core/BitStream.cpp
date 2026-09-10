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

#include "titan/core/Types.cpp"

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

// BitStream — reversed from libg_decrypted.so (ARM64).
// LSB-first packing; see include header for the layout reference.


#include <algorithm>
#include <stdexcept>

namespace titan {

inline BitStream::BitStream(i32 capacity) {
    buffer_.assign(capacity > 0 ? static_cast<std::size_t>(capacity) : 1u, 0);
    byteIndex_ = 0;
    length2_ = 0;
    bitOffset_ = 0;
}

inline BitStream::~BitStream() = default;

inline void BitStream::setBuffer(const u8* data, i32 len) {
    if (len < 0 || data == nullptr) throw std::invalid_argument("BitStream::setBuffer");
    buffer_.assign(data, data + len);
    byteIndex_ = 0;
    length2_ = len;
    bitOffset_ = 0;
}

inline i32 BitStream::getLength() const {
    return std::max(byteIndex_ + 1, length2_);
}

// ensureCapacity @0x624e28: if byteIndex+6 > capacity: capacity += 105.
inline void BitStream::ensureCapacity() {
    if (byteIndex_ + 6 > static_cast<i32>(buffer_.size())) {
        std::vector<u8> grown(buffer_.size() + 105u, 0);
        for (i32 i = 0; i <= byteIndex_ && i < static_cast<i32>(buffer_.size()); ++i) {
            grown[static_cast<std::size_t>(i)] = buffer_[static_cast<std::size_t>(i)];
        }
        buffer_.swap(grown);
    }
}

inline void BitStream::writeBit(bool b) {
    ensureCapacity();
    if (b) {
        buffer_[static_cast<std::size_t>(byteIndex_)] |= static_cast<u8>(1u << bitOffset_);
    }
    ++bitOffset_;
    if (bitOffset_ == 8) {
        bitOffset_ = 0;
        ++byteIndex_;
        ensureCapacity();
        buffer_[static_cast<std::size_t>(byteIndex_)] = 0;
    }
    if (byteIndex_ + 1 > length2_) length2_ = byteIndex_ + 1;
}

inline bool BitStream::readBit() {
    if (byteIndex_ >= static_cast<i32>(buffer_.size())) {
        throw std::out_of_range("BitStream: read past end");
    }
    const bool b =
        ((buffer_[static_cast<std::size_t>(byteIndex_)] >> bitOffset_) & 1u) != 0;
    ++bitOffset_;
    if (bitOffset_ == 8) {
        bitOffset_ = 0;
        ++byteIndex_;
    }
    return b;
}

// writeBoolean @0x23ca7c
inline void BitStream::writeBoolean(bool v) {
    writePositiveInt(v ? 1 : 0, 1);
}

// readBoolean @0x54dc78
inline bool BitStream::readBoolean() {
    return readPositiveInt(1) != 0;
}

// writePositiveInt @0x8d18d0: clamp [0, 2^bits-1], LSB-first.
inline void BitStream::writePositiveInt(i32 v, int bits) {
    const i32 max = (bits >= 31) ? 0x7FFFFFFF : (~(-1 << bits));
    if (v < 0) v = 0;
    if (v > max) v = max;
    for (int i = 0; i < bits; ++i) writeBit(((v >> i) & 1) != 0);
}

inline i32 BitStream::readPositiveInt(int bits) {
    i32 v = 0;
    for (int i = 0; i < bits; ++i) {
        if (readBit()) v |= (1 << i);
    }
    return v;
}

// writeInt @0x53bd24: clamp [-2^bits+1, 2^bits-1]; sign first (1 => >=0),
// then magnitude LSB-first. Total 1+bits.
inline void BitStream::writeInt(i32 v, int bits) {
    const i32 mag = (bits >= 31) ? 0x7FFFFFFF : (~(-1 << bits));
    if (v < -mag) v = -mag; // binary clamps to [(-1<<bits)+1, ~(-1<<bits)]
    if (v > mag) v = mag;
    writeBit(v >= 0);
    const i32 m = (v >= 0) ? v : -v;
    for (int i = 0; i < bits; ++i) writeBit(((m >> i) & 1) != 0);
}

// writePositiveVInt @0x53c590: bit-length of v in prefixBits, then v itself.
inline void BitStream::writePositiveVInt(i32 v, int prefixBits) {
    const i32 cap = (prefixBits >= 5) ? 0x7FFFFFFF : (~(-1 << (1 << prefixBits)));
    if (v < 0) v = 0;
    if (v > cap) v = cap;
    int len = 1;
    if (v != 0) {
        len = 0;
        for (i32 t = v; t != 0; t >>= 1) ++len;
    }
    writePositiveInt(len, prefixBits);
    for (int i = 0; i < len; ++i) writeBit(((v >> i) & 1) != 0);
}

inline i32 BitStream::readPositiveVInt(int prefixBits) {
    const int len = readPositiveInt(prefixBits);
    i32 v = 0;
    for (int i = 0; i < len; ++i) {
        if (readBit()) v |= (1 << i);
    }
    return v;
}

} // namespace titan

