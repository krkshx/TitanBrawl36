#pragma once

// AllianceEventStreamEntry (alliance subtype 4) — reversed.
// encode @0x3292e0, decode @0x94bc3c, getStreamEntryType @0x9b1ed0 -> 4.
// Wire: base + vint +64, optional logiclong +48 (bool-present),
// optional string +56 (only if +48 present).

#include "titan/game/StreamEntry.hpp"

namespace titan {

class AllianceEventStreamEntry : public StreamEntry {
public:
    int entryType() const { return 4; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        s.writeVInt(v64_);
        s.writeBoolean(hasId_);
        if (hasId_) {
            id48_.encode(s);
            s.writeString(s56_ ? &*s56_ : nullptr);
        }
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        v64_ = s.readVInt();
        hasId_ = s.readBoolean();
        if (hasId_) {
            id48_ = LogicLong::decode(s);
            s56_ = s.readString();
        }
    }
    i32 v64_ = 0;
    bool hasId_ = false;
    LogicLong id48_;
    std::optional<std::string> s56_;
};

} // namespace titan
