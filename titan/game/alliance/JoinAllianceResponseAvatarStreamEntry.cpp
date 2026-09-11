#pragma once

// JoinAllianceResponseAvatarStreamEntry (avatar subtype 3) — reversed
// from libg_decrypted.so.
// Ctor @0x7c3e44 (size 0x60: base + fields below).
// encode @0x518d2c, decode @0x6d3544.
// Wire: AvatarStreamEntry base + LogicLong +48, nullable string +56
// (-1 length form), DataReference +64, nullable string +88, bool +72,
// bool-prefixed nullable LogicLong +80. (Encoder slot map verified:
// +56 writeString, +128 writeVInt, +152 writeLong.)

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class JoinAllianceResponseAvatarStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 3; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        long48_.encode(s);
        s.writeString(text56_ ? &*text56_ : nullptr);
        ref64_.encode(s);
        s.writeString(text88_ ? &*text88_ : nullptr);
        s.writeBoolean(b72_);
        s.writeBoolean(long80_.has_value());
        if (long80_) long80_->encode(s);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        long48_ = LogicLong::decode(s);
        text56_ = s.readString();
        ref64_ = DataReference::decode(s);
        text88_ = s.readString();
        b72_ = s.readBoolean();
        if (s.readBoolean()) {
            long80_.emplace();
            *long80_ = LogicLong::decode(s);
        } else {
            long80_.reset();
        }
    }
    LogicLong long48_; // +48 (binary holds a pointer; port keeps a value)
    std::optional<std::string> text56_; // +56 (nullable)
    DataReference ref64_; // +64
    std::optional<std::string> text88_; // +88 (nullable)
    bool b72_ = false; // +72
    std::optional<LogicLong> long80_; // +80 (bool-prefixed, binary holds a pointer)
};

} // namespace titan
