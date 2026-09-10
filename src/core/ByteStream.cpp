// ByteStream — reversed from libg_decrypted.so (ARM64).
// IDA addresses are cited per method. See docs/IDA_BASELINE.md.

#include "titan/core/ByteStream.hpp"

#include <cstring>
#include <stdexcept>

namespace titan {

namespace {
// String policy from ByteStream::writeString @0x5174d0.
constexpr i32 kMaxStringBytes = 900000;
} // namespace

ByteStream::ByteStream(std::size_t initialCapacity) {
    buffer_.assign(initialCapacity, 0);
    length_ = 0;
    bitOffset_ = 0;
    readCursor_ = 0;
}

ByteStream::~ByteStream() = default;

void ByteStream::clear() {
    length_ = 0;
    bitOffset_ = 0;
    readCursor_ = 0;
    setChecksum(0);
}

void ByteStream::setBuffer(const u8* data, i32 len) {
    if (len < 0 || data == nullptr) {
        throw std::invalid_argument("ByteStream::setBuffer: null/negative");
    }
    buffer_.assign(data, data + len);
    length_ = len;
    readCursor_ = 0;
    bitOffset_ = 0;
}

void ByteStream::ensureCapacity(i32 extra) {
    const i32 need = length_ + extra;
    if (need > static_cast<i32>(buffer_.size())) {
        // Binary grows by fixed steps (+101/+104/+105 depending on caller);
        // vector growth keeps the same observable wire bytes.
        std::vector<u8> grown(static_cast<std::size_t>(need) + 128u, 0);
        std::memcpy(grown.data(), buffer_.data(), static_cast<std::size_t>(length_));
        buffer_.swap(grown);
    }
}

void ByteStream::putByte(u8 b) {
    buffer_[static_cast<std::size_t>(length_)] = b;
    ++length_;
}

// @0x87bc64 — ByteStream::writeIntToByteArray
// Big-endian 4 bytes; resets bitOffset (+28 = 0); grows capacity by +104.
void ByteStream::writeIntToByteArray(i32 value) {
    bitOffset_ = 0;
    ensureCapacity(4);
    putByte(static_cast<u8>((value >> 24) & 0xFF)); // HIBYTE
    putByte(static_cast<u8>((value >> 16) & 0xFF)); // BYTE2
    putByte(static_cast<u8>((value >> 8) & 0xFF));  // BYTE1
    putByte(static_cast<u8>(value & 0xFF));
}

// @0x3f3bd0 — ByteStream::writeInt
//   ChecksumEncoder::writeInt(a1, v); writeIntToByteArray(a1, v)
void ByteStream::writeInt(i32 value) {
    ChecksumEncoder::writeInt(value);
    writeIntToByteArray(value);
}

// @0x3294c0 — ByteStream::writeBoolean
// Bit-packs into the current byte when bitOffset != 0, else starts a new
// zero byte; bitOffset = (bitOffset + 1) & 7. Checksum first.
void ByteStream::writeBoolean(bool value) {
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
void ByteStream::writeVInt(i32 value) {
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

// @0x5174d0 — ByteStream::writeString
// Checksum with char length; null -> writeInt(-1); length comes from the
// UTF-8 byte length; >= 900001 bytes rejected (-> -1).
void ByteStream::writeString(const std::string* value) {
    if (value == nullptr) {
        ChecksumEncoder::writeStringLength(0, true);
        writeIntToByteArray(-1);
        return;
    }
    const i32 len = static_cast<i32>(value->size());
    ChecksumEncoder::writeStringLength(len, false);
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
void ByteStream::writeStringReference(const std::string& value) {
    const i32 len = static_cast<i32>(value.size());
    ChecksumEncoder::writeStringReferenceLength(len);
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
void ByteStream::writeBytes(const u8* data, i32 len) {
    // Checksum fold for raw bytes mirrors the string path (length term).
    ChecksumEncoder::writeStringLength(len, data == nullptr);
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

void ByteStream::writeByte(i8 value) {
    bitOffset_ = 0;
    ensureCapacity(1);
    putByte(static_cast<u8>(value));
}

void ByteStream::writeShort(i16 value) {
    bitOffset_ = 0;
    ensureCapacity(2);
    putByte(static_cast<u8>((value >> 8) & 0xFF));
    putByte(static_cast<u8>(value & 0xFF));
}

void ByteStream::writeLongLong(i64 value) {
    bitOffset_ = 0;
    ensureCapacity(8);
    for (int shift = 56; shift >= 0; shift -= 8) {
        putByte(static_cast<u8>((value >> shift) & 0xFF));
    }
}

// --- readers ---

u8 ByteStream::readByteRaw() {
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
bool ByteStream::readBoolean() {
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
i32 ByteStream::readInt() {
    bitOffset_ = 0;
    const u32 b0 = readByteRaw();
    const u32 b1 = readByteRaw();
    const u32 b2 = readByteRaw();
    const u32 b3 = readByteRaw();
    return static_cast<i32>((b0 << 24) | (b1 << 16) | (b2 << 8) | b3);
}

i8 ByteStream::readByte() {
    bitOffset_ = 0;
    return static_cast<i8>(readByteRaw());
}

i16 ByteStream::readShort() {
    bitOffset_ = 0;
    const u32 hi = readByteRaw();
    const u32 lo = readByteRaw();
    return static_cast<i16>((hi << 8) | lo);
}

i64 ByteStream::readLongLong() {
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
i32 ByteStream::readVInt() {
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

std::optional<std::string> ByteStream::readString() {
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
std::string ByteStream::readStringReference() {
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

std::optional<std::vector<u8>> ByteStream::readBytesNullable() {
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

std::vector<u8> ByteStream::readBytes() {
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
