#pragma once

// AllianceKickOutStreamEntry (avatar subtype 5) — reversed from libg_decrypted.so.
// Ctor @0x52d2a8 (size 0x58: base + fields below).
// encode @0x6aba2c, decode @0x54bcf0.
// Wire: AvatarStreamEntry base + nullable string +48 (-1 length form),
// LogicLong +56, nullable string +64, DataReference +72, bool-prefixed
// nullable LogicLong +80. (Encoder slot map verified: +56 writeString,
// +152 writeLong.)

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceKickOutStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 5; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeString(text48_ ? &*text48_ : nullptr);
        long56_.encode(s);
        s.writeString(text64_ ? &*text64_ : nullptr);
        ref72_.encode(s);
        s.writeBoolean(long80_.has_value());
        if (long80_) long80_->encode(s);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        text48_ = s.readString();
        long56_ = LogicLong::decode(s);
        text64_ = s.readString();
        ref72_ = DataReference::decode(s);
        if (s.readBoolean()) {
            long80_.emplace();
            *long80_ = LogicLong::decode(s);
        } else {
            long80_.reset();
        }
    }
    std::optional<std::string> text48_; // +48 (nullable)
    LogicLong long56_; // +56 (binary holds a pointer; port keeps a value)
    std::optional<std::string> text64_; // +64 (nullable)
    DataReference ref72_; // +72
    std::optional<LogicLong> long80_; // +80 (bool-prefixed, binary holds a pointer)
};

} // namespace titan
