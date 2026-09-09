// ByteStream — reversed from libg_decrypted.so (ARM64).
// IDA addresses are cited per method. See docs/IDA_BASELINE.md.

#include "titan/core/ByteStream.hpp"

#include <cstring>

namespace titan {

namespace {
// String policy from ByteStream::writeString @0x5174d0.
constexpr i32 kMaxStringBytes = 900000;
} // namespace

ByteStream::ByteStream(std::size_t initialCapacity) {
    buffer_.reserve(initialCapacity);
    buffer_.resize(initialCapacity);
    // Binary keeps spare capacity; length starts at 0.
    buffer_.clear();
    buffer_.resize(initialCapacity);
    length_ = 0;
    bitOffset_ = 0;
}

ByteStream::~ByteStream() = default;

void ByteStream::clear() {
    length_ = 0;
    bitOffset_ = 0;
    setChecksum(0);
}

void ByteStream::ensureCapacity(i32 extra) {
    const i32 need = length_ + extra;
    if (need > static_cast<i32>(buffer_.size())) {
        // Binary grows by fixed steps (+101/+104/+105 depending on caller);
        // vector growth keeps the same observable wire bytes.
        std::vector<u8> grown(buffer_.size() + static_cast<std::size_t>(extra) + 128u, 0);
        std::memcpy(grown.data(), buffer_.data(), static_cast<std::size_t>(length_));
        buffer_.swap(grown);
    }
}

// @0x87bc64 — ByteStream::writeIntToByteArray
// Big-endian 4 bytes; resets bitOffset (+28 = 0); grows capacity by +104.
void ByteStream::writeIntToByteArray(i32 value) {
    bitOffset_ = 0;
    ensureCapacity(4);
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>((value >> 24) & 0xFF); // HIBYTE
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>((value >> 16) & 0xFF); // BYTE2
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>((value >> 8) & 0xFF);  // BYTE1
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>(value & 0xFF);
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
        if (bit) {
            buffer_[static_cast<std::size_t>(length_) - 1u] |= static_cast<u8>(1u << bitOffset_);
        }
    } else {
        ensureCapacity(1);
        buffer_[static_cast<std::size_t>(length_)++] = 0;
        if (bit) {
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

    auto put = [this](u8 b) { buffer_[static_cast<std::size_t>(length_)++] = b; };
    const auto u = static_cast<u32>(value);

    if (value < 0) {
        if (value >= -63) {
            put(static_cast<u8>((value & 0x3F) | 0x40));
            return;
        }
        if (value >= -8191) {
            put(static_cast<u8>(value | 0xC0));
            put(static_cast<u8>((u >> 6) & 0x7F));
            return;
        }
        if (value >= -1048575) {
            put(static_cast<u8>(value | 0xC0));
            put(static_cast<u8>((u >> 6) | 0x80));
            put(static_cast<u8>((u >> 13) & 0x7F));
            return;
        }
        put(static_cast<u8>(value | 0xC0));
        put(static_cast<u8>((u >> 6) | 0x80));
        put(static_cast<u8>((u >> 13) | 0x80));
        if (value >= -134217727) {
            put(static_cast<u8>((u >> 20) & 0x7F));
        } else {
            put(static_cast<u8>((u >> 20) | 0x80));
            put(static_cast<u8>((u >> 27) & 0x0F));
        }
        return;
    }
    if (value > 63) {
        if (value < 0x2000) {
            put(static_cast<u8>((value & 0x3F) | 0x80));
            put(static_cast<u8>((u >> 6) & 0x7F));
            return;
        }
        if (value < 0x100000) {
            put(static_cast<u8>((value & 0x3F) | 0x80));
            put(static_cast<u8>((u >> 6) | 0x80));
            put(static_cast<u8>((u >> 13) & 0x7F));
            return;
        }
        put(static_cast<u8>((value & 0x3F) | 0x80));
        put(static_cast<u8>((u >> 6) | 0x80));
        put(static_cast<u8>((u >> 13) | 0x80));
        if (value <= 0x7FFFFFF) {
            put(static_cast<u8>((u >> 20) & 0x7F));
        } else {
            put(static_cast<u8>((u >> 20) | 0x80));
            put(static_cast<u8>((u >> 27) & 0x0F));
        }
        return;
    }
    put(static_cast<u8>(value & 0x3F));
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
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>(value);
}

void ByteStream::writeShort(i16 value) {
    bitOffset_ = 0;
    ensureCapacity(2);
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>((value >> 8) & 0xFF);
    buffer_[static_cast<std::size_t>(length_)++] = static_cast<u8>(value & 0xFF);
}

void ByteStream::writeLongLong(i64 value) {
    bitOffset_ = 0;
    ensureCapacity(8);
    for (int shift = 56; shift >= 0; shift -= 8) {
        buffer_[static_cast<std::size_t>(length_)++] =
            static_cast<u8>((value >> shift) & 0xFF);
    }
}

} // namespace titan
