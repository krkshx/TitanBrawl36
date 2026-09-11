#pragma once

// AllianceInvitationAvatarStreamEntry (avatar subtype 4) — reversed
// from libg_decrypted.so.
// Ctor @0x2615dc (size 0x50: base + fields below).
// encode @0x8e3dfc, decode @0x8fd7e8.
// Wire: AvatarStreamEntry base + nullable string +48 (-1 length form),
// vint +56, DataReference +64, bool-prefixed nullable string +72.
// The binary caps the +56 read at 900000; the port reads a plain
// vint like everywhere else.

#include "titan/core/DataReference.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceInvitationAvatarStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 4; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeString(name48_ ? &*name48_ : nullptr);
        s.writeVInt(v56_);
        ref64_.encode(s);
        s.writeBoolean(text72_.has_value());
        if (text72_) s.writeString(&*text72_);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        name48_ = s.readString();
        v56_ = s.readVInt();
        ref64_ = DataReference::decode(s);
        text72_ = s.readBoolean() ? s.readString() : std::nullopt;
    }
    std::optional<std::string> name48_; // +48 (nullable)
    i32 v56_ = 0; // +56
    DataReference ref64_; // +64
    std::optional<std::string> text72_; // +72 (bool-prefixed)
};

} // namespace titan
