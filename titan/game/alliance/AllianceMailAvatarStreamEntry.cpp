#pragma once

// AllianceMailAvatarStreamEntry (avatar subtype 6) — reversed from libg_decrypted.so.
// Ctor @0x1c5bc8 (size 0x58: base + fields below).
// encode @0x61c398, decode @0x275804.
// Wire: AvatarStreamEntry base + vint +80 first, then bool-prefixed
// nullable string +48, nullable string +56 (-1 length form), vint +64,
// DataReference +72. On decode a false presence flag leaves +48
// untouched, exactly like the binary. The binary caps the +64/+80
// reads at 900000; the port reads plain vints like everywhere else.

#include "titan/core/DataReference.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceMailAvatarStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 6; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeVInt(v80_);
        s.writeBoolean(name48_.has_value());
        if (name48_) s.writeString(&*name48_);
        s.writeString(text56_ ? &*text56_ : nullptr);
        s.writeVInt(v64_);
        ref72_.encode(s);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        v80_ = s.readVInt();
        if (s.readBoolean()) name48_ = s.readString();
        text56_ = s.readString();
        v64_ = s.readVInt();
        ref72_ = DataReference::decode(s);
    }
    i32 v80_ = 0; // +80 (wired first)
    std::optional<std::string> name48_; // +48 (bool-prefixed)
    std::optional<std::string> text56_; // +56 (nullable)
    i32 v64_ = 0; // +64
    DataReference ref72_; // +72
};

} // namespace titan
