#pragma once

// JoinRequestAllianceStreamEntry (alliance subtype 3) — reversed.
// encode @0x2347e8, decode @0x453350, getStreamEntryType @0xa54fb4 -> 3.
// Wire: base + string +48, string +64, vint +56, PlayerDisplayData +72.

#include "titan/game/PlayerDisplayData.hpp"
#include "titan/game/StreamEntry.hpp"

namespace titan {

class JoinRequestAllianceStreamEntry : public StreamEntry {
public:
    int entryType() const { return 3; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        s.writeString(s48_ ? &*s48_ : nullptr);
        s.writeString(s64_ ? &*s64_ : nullptr);
        s.writeVInt(v56_);
        display_.encode(s);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        s48_ = s.readString();
        s64_ = s.readString();
        v56_ = s.readVInt();
        display_.decode(s);
    }
    std::optional<std::string> s48_, s64_;
    i32 v56_ = 0;
    PlayerDisplayData display_;
};

} // namespace titan
