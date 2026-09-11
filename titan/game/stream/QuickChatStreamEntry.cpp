#pragma once

// QuickChatStreamEntry (alliance subtype 8) — reversed from libg_decrypted.so.
// Ctor @0x2443c8 (size 0x50: base + DataReference +48, LogicLong +56,
// string +64, vints +72/+76). Factory case 8 @0x90d98c.
// encode @0x50960c, decode @0x3f81fc.
// Wire: StreamEntry base + DataReference +48, bool-prefixed nullable
// LogicLong +56, nullable string +64 (-1 length form), vint +72,
// vint +76. (Encoder slot map verified against the ChecksumEncoder
// vtable: +56 writeString, +64 writeBoolean, +128 writeVInt,
// +152 writeLong.)

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/stream/StreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class QuickChatStreamEntry : public StreamEntry {
public:
    int entryType() const { return 8; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        ref48_.encode(s);
        s.writeBoolean(long56_.has_value());
        if (long56_) long56_->encode(s);
        s.writeString(text64_ ? &*text64_ : nullptr);
        s.writeVInt(v72_);
        s.writeVInt(v76_);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        ref48_ = DataReference::decode(s);
        if (s.readBoolean()) {
            long56_.emplace();
            *long56_ = LogicLong::decode(s);
        } else {
            long56_.reset();
        }
        text64_ = s.readString();
        v72_ = s.readVInt();
        v76_ = s.readVInt();
    }
    DataReference ref48_; // +48
    std::optional<LogicLong> long56_; // +56 (bool-prefixed, binary holds a pointer)
    std::optional<std::string> text64_; // +64 (nullable)
    i32 v72_ = 0, v76_ = 0; // +72/+76
};

} // namespace titan
