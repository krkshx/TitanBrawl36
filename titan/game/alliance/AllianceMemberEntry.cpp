#pragma once

// AllianceMemberEntry::encode @0x2367d4, decode @0x64244c.
// Wire: logiclong, vint x2, vint x3, bool, PlayerDisplayData (always present).
// Split out of AllianceEntries.hpp; wire format unchanged.

#include "titan/core/LogicLong.cpp"
#include "titan/game/avatar/PlayerDisplayData.cpp"
#include "titan/messages/Nested.cpp"

namespace titan {

class AllianceMemberEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        s.writeVInt(v40_);
        s.writeVInt(v44_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
        s.writeVInt(v68_);
        s.writeBoolean(flag_);
        display_.encode(s);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        v40_ = s.readVInt();
        v44_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
        v68_ = s.readVInt();
        flag_ = s.readBoolean();
        display_.decode(s);
    }
    LogicLong id_;
    i32 v40_ = 0, v44_ = 0, v60_ = 0, v64_ = 0, v68_ = 0;
    bool flag_ = false;
    PlayerDisplayData display_;
};

} // namespace titan
