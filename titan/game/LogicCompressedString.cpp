// LogicCompressedString RLE — reversed from libg_decrypted.so (ARM64).

#include "titan/game/LogicCompressedString.hpp"

#include <stdexcept>

namespace titan {

void LogicCompressedString::setUncompressed(const u8* data, i32 len) {
    ByteStream tmp;
    compressTo(tmp, data, len);
    data_ = std::vector<u8>(tmp.data(), tmp.data() + tmp.size());
}

std::vector<u8> LogicCompressedString::getUncompressed() const {
    if (!data_) return {};
    ByteStream tmp;
    tmp.setBuffer(data_->data(), static_cast<i32>(data_->size()));
    return decompressFrom(tmp);
}

// compress @0x93c13c: VInt uncomprLen, IntBE comprLen (backpatched), RLE body.
void LogicCompressedString::compressTo(ByteStream& s, const u8* data, i32 len) {
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
std::vector<u8> LogicCompressedString::decompressFrom(ByteStream& s) {
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
