#pragma once

// BattleReportStreamEntry (avatar subtypes 1-2) — reversed from libg_decrypted.so.
// Ctor @0x366780 takes the dynamic type (stored at +44, not on the wire;
// the entry type travels separately like in StreamEntry).
// encode @0x227b54, decode @0x41b4f8.
// Wire: AvatarStreamEntry base, nullable string +48 (-1 length form),
// bool +56, vint +72/+76/+80, then bool-prefixed nullable LogicLong +64
// preceded by vint +60 when present (+60 defaults to -1 in the ctor).
// (Encoder slot map verified: +56 writeString, +128 writeVInt,
// +152 writeLong.)

#include "titan/core/LogicLong.cpp"
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
        s.writeString(text48_ ? &*text48_ : nullptr);
        s.writeBoolean(b56_);
        s.writeVInt(v72_);
        s.writeVInt(v76_);
        s.writeVInt(v80_);
        s.writeBoolean(long64_.has_value());
        if (long64_) {
            s.writeVInt(v60_);
            long64_->encode(s);
        }
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        text48_ = s.readString();
        b56_ = s.readBoolean();
        v72_ = s.readVInt();
        v76_ = s.readVInt();
        v80_ = s.readVInt();
        if (s.readBoolean()) {
            v60_ = s.readVInt();
            long64_.emplace();
            *long64_ = LogicLong::decode(s);
        } else {
            long64_.reset();
        }
    }
    int type44_ = 1; // +44 (ctor arg, dynamic subtype 1-2, not on wire)
    std::optional<std::string> text48_; // +48 (nullable)
    bool b56_ = false; // +56
    i32 v72_ = 0, v76_ = 0, v80_ = 0; // +72/+76/+80
    i32 v60_ = -1; // +60 (only on wire when the long is present)
    std::optional<LogicLong> long64_; // +64 (bool-prefixed, binary holds a pointer)
};

} // namespace titan
