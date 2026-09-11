#pragma once

// AllianceKickOutStreamEntry (avatar subtype 5) — reversed from libg_decrypted.so.
// Ctor @0x52d2a8 (size 0x58: base + fields below).
// encode @0x6aba2c, decode @0x54bcf0.
// Wire: AvatarStreamEntry base + vint +48, nullable string +56
// (-1 length form), vint +64, DataReference +72, bool-prefixed
// nullable string +80. The binary caps the +48/+64 reads at 900000;
// the port reads plain vints like everywhere else.

#include "titan/core/DataReference.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceKickOutStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 5; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeVInt(v48_);
        s.writeString(name56_ ? &*name56_ : nullptr);
        s.writeVInt(v64_);
        ref72_.encode(s);
        s.writeBoolean(text80_.has_value());
        if (text80_) s.writeString(&*text80_);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        v48_ = s.readVInt();
        name56_ = s.readString();
        v64_ = s.readVInt();
        ref72_ = DataReference::decode(s);
        text80_ = s.readBoolean() ? s.readString() : std::nullopt;
    }
    i32 v48_ = 0; // +48
    std::optional<std::string> name56_; // +56 (nullable)
    i32 v64_ = 0; // +64
    DataReference ref72_; // +72
    std::optional<std::string> text80_; // +80 (bool-prefixed)
};

} // namespace titan
