#pragma once

#include "titan/core/ChecksumEncoder.cpp"
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

// UTF-16 length of WTF-8 bytes (LogicStringUtil::getCharLength @0x62b658:
// BMP=1, supplementary=2, invalid=1). Used for checksum folds, which count
// UTF-16 units while the wire carries UTF-8 bytes.
[[nodiscard]] i32 utf16Length(const u8* data, i32 len);
[[nodiscard]] inline i32 utf16Length(const std::string& s) {
    return utf16Length(reinterpret_cast<const u8*>(s.data()),
                       static_cast<i32>(s.size()));
}

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
    void writeVLong(i64 value) override;        // @0x3ec094
    void writeString(const std::string* value); // @0x5174d0 (null -> -1)
    void writeStringReference(const std::string& value); // @0x608f14 (never null)
    void writeBytes(const u8* data, i32 len);   // @0x61bd08 (null -> -1)
    void writeBytesWithoutLength(const u8* data, i32 len); // @0x400d5c
    void writeRawBytes(const u8* data, i32 len); // no length prefix, no checksum
    void writeByte(i8 value);
    void writeShort(i16 value);
    void writeLongLong(i64 value);

    // --- readers ---
    bool readBoolean();                    // @0x23bc10
    i8 readByte();
    i16 readShort();
    i32 readInt();                         // @0x191dc4
    i32 readVInt();                        // @0x356c40
    i64 readVLong();                       // @0x8d3af0
    // Encoded length of writeVLong(value) without writing anything.
    // Threshold table copied from @0x258bb4.
    [[nodiscard]] static int getVLongSizeInBytes(i64 value);
    i64 readLongLong();
    std::optional<std::string> readString();
    std::string readStringReference(); // @0x28f62c (negative len -> "")
    std::vector<u8> readBytes();
    std::vector<u8> readRawBytes(i32 len);
    std::optional<std::vector<u8>> readBytesNullable(); // -1 -> nullopt

    // Load a received buffer for decoding (sets cursor to 0).
    void setBuffer(const u8* data, i32 len);
    void setByteArray(const u8* data, i32 len) { setBuffer(data, len); } // @0x2da99c

    // --- buffer access ---
    [[nodiscard]] const u8* data() const { return buffer_.data(); }
    [[nodiscard]] const u8* getByteArray() const { return buffer_.data(); } // @0x42b9b0
    [[nodiscard]] i32 size() const { return length_; }
    [[nodiscard]] i32 getLength() const { return length_; } // @0x69da1c (max(+20,+24); equal here)
    [[nodiscard]] i32 cursor() const { return readCursor_; }
    [[nodiscard]] i32 remaining() const { return length_ - readCursor_; }
    [[nodiscard]] bool isAtEnd() const { return readCursor_ >= length_; } // @0x7625a8
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

// ByteStream — reversed from libg_decrypted.so (ARM64).
// IDA addresses are cited per method. See docs/IDA_BASELINE.md.


#include <cstring>
#include <stdexcept>

namespace titan {

namespace {
// String policy from ByteStream::writeString @0x5174d0.
constexpr i32 kMaxStringBytes = 900000;
} // namespace

// getCharLength @0x62b658 semantics over WTF-8 input.
inline i32 utf16Length(const u8* data, i32 len) {
    i32 units = 0;
    i32 i = 0;
    while (i < len) {
        const u8 b = data[i];
        if (b < 0x80) {
            ++units;
            ++i;
        } else if ((b & 0xE0) == 0xC0) {
            ++units; // includes WTF-8 surrogates halves (1 each)
            i += 2;
        } else if ((b & 0xF0) == 0xE0) {
            ++units;
            i += 3;
        } else if ((b & 0xF8) == 0xF0) {
            units += 2; // supplementary plane
            i += 4;
        } else {
            ++units; // invalid -> U+FFFD
            ++i;
        }
        if (i > len) break; // truncated tail counts once
    }
    return units;
}

inline ByteStream::ByteStream(std::size_t initialCapacity) {
    buffer_.assign(initialCapacity, 0);
    length_ = 0;
    bitOffset_ = 0;
    readCursor_ = 0;
}

inline ByteStream::~ByteStream() = default;

inline void ByteStream::clear() {
    length_ = 0;
    bitOffset_ = 0;
    readCursor_ = 0;
    setChecksum(0);
}

inline void ByteStream::setBuffer(const u8* data, i32 len) {
    if (len < 0 || data == nullptr) {
        throw std::invalid_argument("ByteStream::setBuffer: null/negative");
    }
    buffer_.assign(data, data + len);
    length_ = len;
    readCursor_ = 0;
    bitOffset_ = 0;
}

inline void ByteStream::ensureCapacity(i32 extra) {
    const i32 need = length_ + extra;
    if (need > static_cast<i32>(buffer_.size())) {
        // Binary grows by fixed steps (+101/+104/+105 depending on caller);
        // vector growth keeps the same observable wire bytes.
        std::vector<u8> grown(static_cast<std::size_t>(need) + 128u, 0);
        std::memcpy(grown.data(), buffer_.data(), static_cast<std::size_t>(length_));
        buffer_.swap(grown);
    }
}

inline void ByteStream::putByte(u8 b) {
    buffer_[static_cast<std::size_t>(length_)] = b;
    ++length_;
}

// @0x87bc64 — ByteStream::writeIntToByteArray
// Big-endian 4 bytes; resets bitOffset (+28 = 0); grows capacity by +104.
inline void ByteStream::writeIntToByteArray(i32 value) {
    bitOffset_ = 0;
    ensureCapacity(4);
    putByte(static_cast<u8>((value >> 24) & 0xFF)); // HIBYTE
    putByte(static_cast<u8>((value >> 16) & 0xFF)); // BYTE2
    putByte(static_cast<u8>((value >> 8) & 0xFF));  // BYTE1
    putByte(static_cast<u8>(value & 0xFF));
}

// @0x3f3bd0 — ByteStream::writeInt
//   ChecksumEncoder::writeInt(a1, v); writeIntToByteArray(a1, v)
inline void ByteStream::writeInt(i32 value) {
    ChecksumEncoder::writeInt(value);
    writeIntToByteArray(value);
}

// @0x3294c0 — ByteStream::writeBoolean
// Bit-packs into the current byte when bitOffset != 0, else starts a new
// zero byte; bitOffset = (bitOffset + 1) & 7. Checksum first.
inline void ByteStream::writeBoolean(bool value) {
    ChecksumEncoder::writeBoolean(value);
    const u8 bit = value ? 1u : 0u;
    if (bitOffset_ != 0) {
        if (bit != 0u) {
            buffer_[static_cast<std::size_t>(length_) - 1u] |= static_cast<u8>(1u << bitOffset_);
        }
    } else {
        ensureCapacity(1);
        putByte(0);
        if (bit != 0u) {
            buffer_[static_cast<std::size_t>(length_) - 1u] |= static_cast<u8>(1u << 0);
        }
    }
    bitOffset_ = (bitOffset_ + 1) & 7;
}

// @0x298f64 — ByteStream::writeVInt
// Checksum first, reset bitOffset, ensure length+5 fits (binary: +105),
// then the 1..5 byte sign-aware varint (see docs/IDA_BASELINE.md).
inline void ByteStream::writeVInt(i32 value) {
    ChecksumEncoder::writeVInt(value);
    bitOffset_ = 0;
    ensureCapacity(5);

    const auto u = static_cast<u32>(value);

    if (value < 0) {
        if (value >= -63) {
            putByte(static_cast<u8>((value & 0x3F) | 0x40));
            return;
        }
        if (value >= -8191) {
            putByte(static_cast<u8>(value | 0xC0));
            putByte(static_cast<u8>((u >> 6) & 0x7F));
            return;
        }
        if (value >= -1048575) {
            putByte(static_cast<u8>(value | 0xC0));
            putByte(static_cast<u8>((u >> 6) | 0x80));
            putByte(static_cast<u8>((u >> 13) & 0x7F));
            return;
        }
        putByte(static_cast<u8>(value | 0xC0));
        putByte(static_cast<u8>((u >> 6) | 0x80));
        putByte(static_cast<u8>((u >> 13) | 0x80));
        if (value >= -134217727) {
            putByte(static_cast<u8>((u >> 20) & 0x7F));
        } else {
            putByte(static_cast<u8>((u >> 20) | 0x80));
            putByte(static_cast<u8>((u >> 27) & 0x0F));
        }
        return;
    }
    if (value > 63) {
        if (value < 0x2000) {
            putByte(static_cast<u8>((value & 0x3F) | 0x80));
            putByte(static_cast<u8>((u >> 6) & 0x7F));
            return;
        }
        if (value < 0x100000) {
            putByte(static_cast<u8>((value & 0x3F) | 0x80));
            putByte(static_cast<u8>((u >> 6) | 0x80));
            putByte(static_cast<u8>((u >> 13) & 0x7F));
            return;
        }
        putByte(static_cast<u8>((value & 0x3F) | 0x80));
        putByte(static_cast<u8>((u >> 6) | 0x80));
        putByte(static_cast<u8>((u >> 13) | 0x80));
        if (value <= 0x7FFFFFF) {
            putByte(static_cast<u8>((u >> 20) & 0x7F));
        } else {
            putByte(static_cast<u8>((u >> 20) | 0x80));
            putByte(static_cast<u8>((u >> 27) & 0x0F));
        }
        return;
    }
    putByte(static_cast<u8>(value & 0x3F));
}

// @0x3ec094 — ByteStream::writeVLong
// Checksum first, reset bitOffset, ensure length+10 fits (binary: +110),
// then the 1..10 byte sign-aware varint: first byte carries 6 payload bits
// + 0x40 sign + 0x80 continuation, following bytes 7-bit LE groups with
// 0x80 continuation (verified against the binary's range thresholds:
// 63/-64, 0x2000/-8191, 0x100000/-1048575, 0x7FFFFFF/-134217727, ...).
inline void ByteStream::writeVLong(i64 value) {
    ChecksumEncoder::writeVLong(value);
    bitOffset_ = 0;
    ensureCapacity(10);

    // Length is range-selected (NOT remaining-bits): the binary emits the
    // canonical long form at exact powers (e.g. -8192 takes 3 bytes).
    const int n = getVLongSizeInBytes(value);
    const u64 u = static_cast<u64>(value);
    const bool neg = value < 0;
    if (n == 1) {
        putByte(static_cast<u8>((u & 0x3F) | (neg ? 0x40u : 0u)));
        return;
    }
    putByte(static_cast<u8>((u & 0x3F) | 0x80 | (neg ? 0x40u : 0u)));
    int shift = 6;
    for (int i = 2; i < n; ++i) {
        putByte(static_cast<u8>(((u >> shift) & 0x7F) | 0x80));
        shift += 7;
    }
    putByte(static_cast<u8>((u >> shift) & 0x7F));
}

// @0x5174d0 — ByteStream::writeString
// Checksum with char length; null -> writeInt(-1); length comes from the
// UTF-8 byte length; >= 900001 bytes rejected (-> -1).
inline void ByteStream::writeString(const std::string* value) {
    if (value == nullptr) {
        ChecksumEncoder::writeStringLength(0, true);
        writeIntToByteArray(-1);
        return;
    }
    const i32 len = static_cast<i32>(value->size());
    ChecksumEncoder::writeStringLength(utf16Length(*value), false);
    if (len >= kMaxStringBytes + 1) { // ByteLength < 900001 required
        writeIntToByteArray(-1);
        return;
    }
    ensureCapacity(len + 4);
    writeIntToByteArray(len);
    if (len > 0) {
        std::memcpy(buffer_.data() + length_, value->data(), static_cast<std::size_t>(len));
        length_ += len;
    }
}

// @0x608f14 — ByteStream::writeStringReference
// Same wire layout as writeString but the reference is never null:
// ByteLength < 900001 required, else Debugger::warning + writeInt(-1).
// Checksum fold uses K=38 (@0x69a564).
inline void ByteStream::writeStringReference(const std::string& value) {
    const i32 len = static_cast<i32>(value.size());
    ChecksumEncoder::writeStringReferenceLength(utf16Length(value));
    if (len >= kMaxStringBytes + 1) {
        writeIntToByteArray(-1);
        return;
    }
    ensureCapacity(len + 4);
    writeIntToByteArray(len);
    if (len > 0) {
        std::memcpy(buffer_.data() + length_, value.data(), static_cast<std::size_t>(len));
        length_ += len;
    }
}

// @0x61bd08 — ByteStream::writeBytes
// ChecksumEncoder::writeBytes; null -> writeInt(-1),
// else writeInt(len) + raw bytes.
inline void ByteStream::writeBytes(const u8* data, i32 len) {
    ChecksumEncoder::writeBytesLength(len, data == nullptr);
    if (data == nullptr) {
        writeIntToByteArray(-1);
        return;
    }
    ensureCapacity(len + 4);
    writeIntToByteArray(len);
    if (len > 0) {
        std::memcpy(buffer_.data() + length_, data, static_cast<std::size_t>(len));
        length_ += len;
    }
}

// @0x400d5c — ByteStream::writeBytesWithoutLength
// ChecksumEncoder::writeBytes fold, then raw copy (no length prefix,
// bitOffset untouched, growth +100).
inline void ByteStream::writeBytesWithoutLength(const u8* data, i32 len) {
    ChecksumEncoder::writeBytesLength(len, data == nullptr);
    if (data == nullptr) return;
    ensureCapacity(len);
    if (len > 0) {
        std::memcpy(buffer_.data() + length_, data, static_cast<std::size_t>(len));
        length_ += len;
    }
}

inline void ByteStream::writeByte(i8 value) {
    bitOffset_ = 0;
    ensureCapacity(1);
    putByte(static_cast<u8>(value));
}

// Raw payload without length prefix (UdpBigMessageFragment pattern).
inline void ByteStream::writeRawBytes(const u8* data, i32 len) {
    if (len <= 0) return;
    ensureCapacity(len);
    std::memcpy(buffer_.data() + length_, data, static_cast<std::size_t>(len));
    length_ += len;
}

inline void ByteStream::writeShort(i16 value) {
    bitOffset_ = 0;
    ensureCapacity(2);
    putByte(static_cast<u8>((value >> 8) & 0xFF));
    putByte(static_cast<u8>(value & 0xFF));
}

inline void ByteStream::writeLongLong(i64 value) {
    bitOffset_ = 0;
    ensureCapacity(8);
    for (int shift = 56; shift >= 0; shift -= 8) {
        putByte(static_cast<u8>((value >> shift) & 0xFF));
    }
}

// --- readers ---

inline u8 ByteStream::readByteRaw() {
    if (readCursor_ >= length_) {
        throw std::out_of_range("ByteStream: read past end");
    }
    const u8 b = buffer_[static_cast<std::size_t>(readCursor_)];
    ++readCursor_;
    return b;
}

// @0x23bc10 — ByteStream::readBoolean
// offset += (8 - bitOffset) >> 3 (i.e. +1 when starting a fresh byte),
// test bit bitOffset of the current byte, bitOffset = (bitOffset+1) & 7.
inline bool ByteStream::readBoolean() {
    const i32 bit = bitOffset_;
    readCursor_ += (8 - bit) >> 3;
    if (readCursor_ <= 0 || readCursor_ > length_) {
        throw std::out_of_range("ByteStream::readBoolean past end");
    }
    const u8 cur = buffer_[static_cast<std::size_t>(readCursor_) - 1u];
    bitOffset_ = (bit + 1) & 7;
    return ((1u << bit) & cur) != 0;
}

// @0x191dc4 — ByteStream::readInt (big-endian, resets bitOffset).
inline i32 ByteStream::readInt() {
    bitOffset_ = 0;
    const u32 b0 = readByteRaw();
    const u32 b1 = readByteRaw();
    const u32 b2 = readByteRaw();
    const u32 b3 = readByteRaw();
    return static_cast<i32>((b0 << 24) | (b1 << 16) | (b2 << 8) | b3);
}

inline i8 ByteStream::readByte() {
    bitOffset_ = 0;
    return static_cast<i8>(readByteRaw());
}

inline i16 ByteStream::readShort() {
    bitOffset_ = 0;
    const u32 hi = readByteRaw();
    const u32 lo = readByteRaw();
    return static_cast<i16>((hi << 8) | lo);
}

inline i64 ByteStream::readLongLong() {
    bitOffset_ = 0;
    i64 v = 0;
    for (int i = 0; i < 8; ++i) v = (v << 8) | readByteRaw();
    return v;
}

// @0x356c40 — ByteStream::readVInt.
// First byte: low 6 bits payload, 0x40 = negative, 0x80 = continuation.
// Continuation bytes carry 7 bits; the negative path sign-extends the
// result at every length (0xFFFFFFC0 / 0xFFFFE000 / 0xFFF00000 /
// 0xF8000000 / 0x80000000), mirroring the binary exactly.
inline i32 ByteStream::readVInt() {
    bitOffset_ = 0;
    const u8 first = readByteRaw();
    u32 result = first & 0x3F;
    if ((first & 0x40) != 0) { // negative
        if ((first & 0x80) == 0) {
            return static_cast<i32>(first | 0xFFFFFFC0u);
        }
        const u8 b1 = readByteRaw();
        u32 v = (result & 0xFFFFE03Fu) | ((static_cast<u32>(b1 & 0x7F)) << 6);
        if ((b1 & 0x80) == 0) {
            return static_cast<i32>(v | 0xFFFFE000u);
        }
        const u8 b2 = readByteRaw();
        v = (v & 0xFFF01FFFu) | ((static_cast<u32>(b2 & 0x7F)) << 13);
        if ((b2 & 0x80) == 0) {
            return static_cast<i32>(v | 0xFFF00000u);
        }
        const u8 b3 = readByteRaw();
        v = (v & 0xF80FFFFFu) | ((static_cast<u32>(b3 & 0x7F)) << 20);
        if ((b3 & 0x80) == 0) {
            return static_cast<i32>(v | 0xF8000000u);
        }
        const u8 b4 = readByteRaw();
        return static_cast<i32>((v & 0x07FFFFFFu) |
                                (static_cast<u32>(b4) << 27) | 0x80000000u);
    }
    if ((first & 0x80) == 0) {
        return static_cast<i32>(result);
    }
    const u8 b1 = readByteRaw();
    result = (result & 0xFFFFE03Fu) | ((static_cast<u32>(b1 & 0x7F)) << 6);
    if ((b1 & 0x80) == 0) {
        return static_cast<i32>(result);
    }
    const u8 b2 = readByteRaw();
    result = (result & 0xFFF01FFFu) | ((static_cast<u32>(b2 & 0x7F)) << 13);
    if ((b2 & 0x80) == 0) {
        return static_cast<i32>(result);
    }
    const u8 b3 = readByteRaw();
    result = (result & 0xF80FFFFFu) | ((static_cast<u32>(b3 & 0x7F)) << 20);
    if ((b3 & 0x80) == 0) {
        return static_cast<i32>(result);
    }
    const u8 b4 = readByteRaw();
    return static_cast<i32>((result & 0x07FFFFFFu) | (static_cast<u32>(b4) << 27));
}

// @0x8d3af0 — ByteStream::readVLong.
// 64-bit sibling of readVInt: first byte carries 6 payload bits +
// 0x40 sign + 0x80 continuation, then 7-bit LE groups (up to 10 bytes
// total; the 10th byte is unconditionally final, mirroring the binary's
// unrolled decoder). Negatives are sign-extended per length.
inline i64 ByteStream::readVLong() {
    bitOffset_ = 0;
    const u8 first = readByteRaw();
    u64 v = first & 0x3F;
    const bool neg = (first & 0x40) != 0;
    int total = 6;
    if ((first & 0x80) != 0) {
        int shift = 6;
        for (int i = 0; i < 9; ++i) {
            const u8 b = readByteRaw();
            v |= (static_cast<u64>(b & 0x7F)) << shift;
            shift += 7;
            if ((b & 0x80) == 0) break;
        }
        total = shift;
    }
    if (neg) {
        if (total > 64) {
            v |= (1ULL << 63);
        } else {
            v |= ~((1ULL << total) - 1ULL);
        }
    }
    return static_cast<i64>(v);
}

// @0x258bb4 — ByteStream::getVLongSizeInBytes (static in our port; the
// binary takes an unused this). Threshold table copied exactly.
inline int ByteStream::getVLongSizeInBytes(i64 value) {
    if (value < 0) {
        if (value > -64) return 1;
        if (value > -8192) return 2;
        if (value > -1048576) return 3;
        if (value > -134217728) return 4;
        if (value > (i64)0xFFFFFFFC00000000LL) return 5;
        if (value > (i64)0xFFFFFE0000000000LL) return 6;
        if (value > (i64)0xFFFF000000000000LL) return 7;
        if (value > (i64)0xFF80000000000000LL) return 8;
        if (value > (i64)0xC000000000000000LL) return 9;
        return 10;
    }
    if (value < 64) return 1;
    if (value < 0x2000) return 2;
    if (value < 0x100000) return 3;
    if (value < 0x8000000) return 4;
    if (value < 0x400000000LL) return 5;
    if (value < 0x20000000000LL) return 6;
    if (value < 0x1000000000000LL) return 7;
    if (value < 0x80000000000000LL) return 8;
    if (value < 0x4000000000000000LL) return 9;
    return 10;
}

inline std::optional<std::string> ByteStream::readString() {
    const i32 len = readInt();
    if (len < 0) {
        return std::nullopt;
    }
    if (readCursor_ + len > length_) {
        throw std::out_of_range("ByteStream::readString past end");
    }
    std::string out(reinterpret_cast<const char*>(buffer_.data() + readCursor_),
                    static_cast<std::size_t>(len));
    readCursor_ += len;
    return out;
}

// @0x28f62c — ByteStream::readStringReference
// readInt length; negative -> Debugger::warning + empty string (never null).
inline std::string ByteStream::readStringReference() {
    const i32 len = readInt();
    if (len < 0) {
        return {};
    }
    if (readCursor_ + len > length_) {
        throw std::out_of_range("ByteStream::readStringReference past end");
    }
    std::string out(reinterpret_cast<const char*>(buffer_.data() + readCursor_),
                    static_cast<std::size_t>(len));
    readCursor_ += len;
    return out;
}

inline std::optional<std::vector<u8>> ByteStream::readBytesNullable() {
    const i32 len = readInt();
    if (len < 0) {
        return std::nullopt;
    }
    if (readCursor_ + len > length_) {
        throw std::out_of_range("ByteStream::readBytes past end");
    }
    std::vector<u8> out(buffer_.begin() + readCursor_, buffer_.begin() + readCursor_ + len);
    readCursor_ += len;
    return out;
}

inline std::vector<u8> ByteStream::readRawBytes(i32 len) {
    if (len < 0) {
        throw std::out_of_range("ByteStream::readRawBytes negative len");
    }
    if (readCursor_ + len > length_) {
        throw std::out_of_range("ByteStream::readRawBytes past end");
    }
    std::vector<u8> out(buffer_.begin() + readCursor_, buffer_.begin() + readCursor_ + len);
    readCursor_ += len;
    return out;
}

inline std::vector<u8> ByteStream::readBytes() {
    const i32 len = readInt();
    if (len < 0) {
        return {};
    }
    if (readCursor_ + len > length_) {
        throw std::out_of_range("ByteStream::readBytes past end");
    }
    std::vector<u8> out(buffer_.begin() + readCursor_, buffer_.begin() + readCursor_ + len);
    readCursor_ += len;
    return out;
}

} // namespace titan

