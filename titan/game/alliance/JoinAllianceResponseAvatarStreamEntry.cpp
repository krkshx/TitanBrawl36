#pragma once

// JoinAllianceResponseAvatarStreamEntry (avatar subtype 3) — reversed
// from libg_decrypted.so.
// Ctor @0x7c3e44 (size 0x60: base + fields below).
// encode @0x518d2c, decode @0x6d3544.
// Wire: AvatarStreamEntry base + nullable string +48 (-1 length form),
// vint +56, DataReference +64, vint +88, bool +72, bool-prefixed
// nullable string +80. The binary caps the +56/+88 reads at 900000;
// the port reads plain vints like everywhere else.

#include "titan/core/DataReference.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class JoinAllianceResponseAvatarStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 3; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeString(name48_ ? &*name48_ : nullptr);
        s.writeVInt(v56_);
        ref64_.encode(s);
        s.writeVInt(v88_);
        s.writeBoolean(b72_);
        s.writeBoolean(text80_.has_value());
        if (text80_) s.writeString(&*text80_);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        name48_ = s.readString();
        v56_ = s.readVInt();
        ref64_ = DataReference::decode(s);
        v88_ = s.readVInt();
        b72_ = s.readBoolean();
        text80_ = s.readBoolean() ? s.readString() : std::nullopt;
    }
    std::optional<std::string> name48_; // +48 (nullable)
    i32 v56_ = 0; // +56
    DataReference ref64_; // +64
    i32 v88_ = 0; // +88
    bool b72_ = false; // +72
    std::optional<std::string> text80_; // +80 (bool-prefixed)
};

} // namespace titan
