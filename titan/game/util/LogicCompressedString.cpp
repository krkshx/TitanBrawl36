#pragma once

// LogicCompressedString + custom RLE — reversed from libg_decrypted.so (ARM64).
//
// LogicCompressedString::encode @0x2b8b80 / decode @0x5ccf08 is a single
// bytes field: writeBytes(intBE len + raw). The payload itself is the RLE
// format below (compress @0x93c13c, decompress @0x43760c):
//   VInt uncompressedLen, IntBE compressedLen, then RLE bytes:
//     b&0xC0 != 0xC0 -> literal byte b
//     else run = b&0x3F, value = next byte, emit value run times.
// Encoder: run>1 or value>=0xC0 -> writeByte(run|0xC0)+writeByte(value);
// singleton <0xC0 as a raw byte; runs capped at 63.

#include "titan/messages/Nested.cpp"

#include <cstddef>
#include <string>
#include <vector>

namespace titan {

class LogicCompressedString : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        if (!data_) {
            s.writeBytes(nullptr, 0);
            return;
        }
        const auto& p = *data_;
        s.writeBytes(p.data(), static_cast<i32>(p.size()));
    }
    void decode(ByteStream& s) override { data_ = s.readBytesNullable(); }

    // Raw (already-compressed) payload access.
    const std::optional<std::vector<u8>>& data() const { return data_; }
    void setData(std::vector<u8> v) { data_ = std::move(v); }

    // Convenience: RLE round-trip of Session/plain bytes.
    void setUncompressed(const u8* data, i32 len);
    void setUncompressed(const std::string& str) {
        setUncompressed(reinterpret_cast<const u8*>(str.data()),
                        static_cast<i32>(str.size()));
    }
    std::vector<u8> getUncompressed() const;

    // Stream-level RLE (compress @0x93c13c / decompress @0x43760c).
    static void compressTo(ByteStream& s, const u8* data, i32 len);
    static std::vector<u8> decompressFrom(ByteStream& s);

private:
    std::optional<std::vector<u8>> data_;
};

} // namespace titan

// LogicCompressedString RLE — reversed from libg_decrypted.so (ARM64).


#include <stdexcept>

namespace titan {

inline void LogicCompressedString::setUncompressed(const u8* data, i32 len) {
    ByteStream tmp;
    compressTo(tmp, data, len);
    data_ = std::vector<u8>(tmp.data(), tmp.data() + tmp.size());
}

inline std::vector<u8> LogicCompressedString::getUncompressed() const {
    if (!data_) return {};
    ByteStream tmp;
    tmp.setBuffer(data_->data(), static_cast<i32>(data_->size()));
    return decompressFrom(tmp);
}

// compress @0x93c13c: VInt uncomprLen, IntBE comprLen (backpatched), RLE body.
inline void LogicCompressedString::compressTo(ByteStream& s, const u8* data, i32 len) {
    s.writeVInt(len);
    // Reserve space for the IntBE compressed length; backpatch afterwards.
    ByteStream body;
    i32 i = 0;
    while (i < len) {
        const u8 val = data[i];
        i32 run = 1;
        while (i + run < len && data[i + run] == val && run < 63) ++run;
        if (run > 1 || val >= 0xC0) {
            body.writeByte(static_cast<i8>(run | 0xC0));
            body.writeByte(static_cast<i8>(val));
        } else {
            body.writeByte(static_cast<i8>(val));
        }
        i += run;
    }
    s.writeInt(body.size());
    if (body.size() > 0) s.writeRawBytes(body.data(), body.size());
}

// decompress @0x43760c: VInt uncomprLen, IntBE comprLen, RLE body; the
// result length must equal uncomprLen.
inline std::vector<u8> LogicCompressedString::decompressFrom(ByteStream& s) {
    const i32 uncomprLen = s.readVInt();
    const i32 comprLen = s.readInt();
    std::vector<u8> out;
    out.reserve(uncomprLen > 0 ? static_cast<std::size_t>(uncomprLen) : 0);
    i32 read = 0;
    while (read < comprLen) {
        const u8 b = static_cast<u8>(s.readByte());
        ++read;
        if ((b & 0xC0) != 0xC0) {
            out.push_back(b);
        } else {
            const i32 run = b & 0x3F;
            const u8 val = static_cast<u8>(s.readByte());
            ++read;
            for (i32 k = 0; k < run; ++k) out.push_back(val);
        }
    }
    if (static_cast<i32>(out.size()) != uncomprLen) {
        throw std::runtime_error("LogicCompressedString::decompress length mismatch");
    }
    return out;
}

} // namespace titan
