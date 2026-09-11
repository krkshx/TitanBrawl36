#pragma once

// QuickChatStreamEntry (alliance subtype 8) — reversed from libg_decrypted.so.
// Ctor @0x2443c8 (size 0x50: base + DataReference +48, string +56,
// vints +64/+72/+76). Factory case 8 @0x90d98c.
// encode @0x50960c, decode @0x3f81fc.
// Wire: StreamEntry base + DataReference +48, bool + nullable string +56
// (explicit presence flag, not the -1 length form), vint +64, vint +72,
// vint +76. The binary caps the +64 read at 900000; the port reads a
// plain vint like everywhere else.

#include "titan/core/DataReference.cpp"
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
        s.writeBoolean(text56_.has_value());
        if (text56_) s.writeString(&*text56_);
        s.writeVInt(v64_);
        s.writeVInt(v72_);
        s.writeVInt(v76_);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        ref48_ = DataReference::decode(s);
        text56_ = s.readBoolean() ? s.readString() : std::nullopt;
        v64_ = s.readVInt();
        v72_ = s.readVInt();
        v76_ = s.readVInt();
    }
    DataReference ref48_; // +48
    std::optional<std::string> text56_; // +56 (bool-prefixed)
    i32 v64_ = 0, v72_ = 0, v76_ = 0; // +64/+72/+76
};

} // namespace titan
