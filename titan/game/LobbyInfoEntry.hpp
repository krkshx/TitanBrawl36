#pragma once

// LobbyInfoEntry::encode @0x263348.
// Wire: 5 × vint @+0..+16.
// Decode mirrors the same order (stream ctor used by LobbyInfoMessage::decode).

#include "titan/messages/Nested.cpp"

namespace titan {

class LobbyInfoEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : v_) s.writeVInt(v);
    }
    void decode(ByteStream& s) override {
        for (i32& v : v_) v = s.readVInt();
    }

    i32 v_[5] = {};
};

} // namespace titan
