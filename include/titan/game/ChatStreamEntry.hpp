#pragma once

// ChatStreamEntry (alliance subtype 2) — reversed from libg_decrypted.so.
// encode @0x929d4c, decode @0x2a02a4, getStreamEntryType @0x996d78 -> 2.
// Wire: StreamEntry base + string +48.

#include "titan/game/StreamEntry.hpp"

namespace titan {

class ChatStreamEntry : public StreamEntry {
public:
    int entryType() const { return 2; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        s.writeString(text_ ? &*text_ : nullptr);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        text_ = s.readString();
    }
    std::optional<std::string> text_;
};

} // namespace titan
