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

#include "titan/messages/Nested.hpp"

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
