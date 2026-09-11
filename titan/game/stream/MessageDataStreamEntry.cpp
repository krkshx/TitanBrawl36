#pragma once

// MessageDataStreamEntry (alliance subtype 6) — reversed from libg_decrypted.so.
// Ctor @0x72876c (size 0x38: base + one DataReference at +48).
// encode @0x68dab0, decode @0x462ee4.
// Wire: StreamEntry base + DataReference +48 (non-nullable).

#include "titan/core/DataReference.cpp"
#include "titan/game/stream/StreamEntry.cpp"

namespace titan {

class MessageDataStreamEntry : public StreamEntry {
public:
    int entryType() const { return 6; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        ref48_.encode(s);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        ref48_ = DataReference::decode(s);
    }
    DataReference ref48_; // +48
};

} // namespace titan
