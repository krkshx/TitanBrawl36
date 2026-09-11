#pragma once

// AllianceMailAvatarStreamEntry (avatar subtype 6) — reversed from libg_decrypted.so.
// Ctor @0x1c5bc8 (size 0x58: base + fields below).
// encode @0x61c398, decode @0x275804.
// Wire: AvatarStreamEntry base + nullable string +80 first (-1 length
// form), then bool-prefixed nullable LogicLong +48, LogicLong +56,
// nullable string +64, DataReference +72. On decode a false presence
// flag leaves +48 untouched, exactly like the binary. (Encoder slot
// map verified: +56 writeString, +152 writeLong.)

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceMailAvatarStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 6; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeString(text80_ ? &*text80_ : nullptr);
        s.writeBoolean(long48_.has_value());
        if (long48_) long48_->encode(s);
        long56_.encode(s);
        s.writeString(text64_ ? &*text64_ : nullptr);
        ref72_.encode(s);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        text80_ = s.readString();
        if (s.readBoolean()) {
            long48_.emplace();
            *long48_ = LogicLong::decode(s);
        }
        long56_ = LogicLong::decode(s);
        text64_ = s.readString();
        ref72_ = DataReference::decode(s);
    }
    std::optional<std::string> text80_; // +80 (wired first, nullable)
    std::optional<LogicLong> long48_; // +48 (bool-prefixed, sticky on false)
    LogicLong long56_; // +56 (binary holds a pointer; port keeps a value)
    std::optional<std::string> text64_; // +64 (nullable)
    DataReference ref72_; // +72
};

} // namespace titan
