#pragma once

// AllianceInvitationAvatarStreamEntry (avatar subtype 4) — reversed
// from libg_decrypted.so.
// Ctor @0x2615dc (size 0x50: base + fields below).
// encode @0x8e3dfc, decode @0x8fd7e8.
// Wire: AvatarStreamEntry base + LogicLong +48, nullable string +56
// (-1 length form), DataReference +64, bool-prefixed nullable
// LogicLong +72. (Encoder slot map verified: +56 writeString,
// +152 writeLong.)

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceInvitationAvatarStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 4; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        long48_.encode(s);
        s.writeString(text56_ ? &*text56_ : nullptr);
        ref64_.encode(s);
        s.writeBoolean(long72_.has_value());
        if (long72_) long72_->encode(s);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        long48_ = LogicLong::decode(s);
        text56_ = s.readString();
        ref64_ = DataReference::decode(s);
        if (s.readBoolean()) {
            long72_.emplace();
            *long72_ = LogicLong::decode(s);
        } else {
            long72_.reset();
        }
    }
    LogicLong long48_; // +48 (binary holds a pointer; port keeps a value)
    std::optional<std::string> text56_; // +56 (nullable)
    DataReference ref64_; // +64
    std::optional<LogicLong> long72_; // +72 (bool-prefixed, binary holds a pointer)
};

} // namespace titan
