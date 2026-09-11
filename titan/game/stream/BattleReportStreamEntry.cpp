#pragma once

// BattleReportStreamEntry (avatar subtypes 1-2) — reversed from libg_decrypted.so.
// Ctor @0x366780 takes the dynamic type (stored at +44, not on the wire;
// the entry type travels separately like in StreamEntry).
// encode @0x227b54, decode @0x41b4f8.
// Wire: AvatarStreamEntry base, vint +48, bool +56, vint +72/+76/+80,
// then bool-prefixed nullable string +64 preceded by vint +60 when present
// (+60 defaults to -1 in the ctor). The binary caps the +48 read at
// 900000; the port reads a plain vint like everywhere else.

#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class BattleReportStreamEntry : public AvatarStreamEntry {
public:
    explicit BattleReportStreamEntry(int type = 1) : type44_(type) {}
    int entryType() const { return type44_; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeVInt(v48_);
        s.writeBoolean(b56_);
        s.writeVInt(v72_);
        s.writeVInt(v76_);
        s.writeVInt(v80_);
        s.writeBoolean(text64_.has_value());
        if (text64_) {
            s.writeVInt(v60_);
            s.writeString(&*text64_);
        }
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        v48_ = s.readVInt();
        b56_ = s.readBoolean();
        v72_ = s.readVInt();
        v76_ = s.readVInt();
        v80_ = s.readVInt();
        if (s.readBoolean()) {
            v60_ = s.readVInt();
            text64_ = s.readString();
        } else {
            text64_.reset();
        }
    }
    int type44_ = 1; // +44 (ctor arg, dynamic subtype 1-2, not on wire)
    i32 v48_ = 0; // +48
    bool b56_ = false; // +56
    i32 v72_ = 0, v76_ = 0, v80_ = 0; // +72/+76/+80
    i32 v60_ = -1; // +60 (only on wire when the string is present)
    std::optional<std::string> text64_; // +64 (bool-prefixed)
};

} // namespace titan
