#pragma once

// AllianceTeamEntry::encode @0x538d6c, decode @0x747a68.
// Wire: vint +0/+4, logiclong +8, vint +16, logiclong +24/+32 (vint pairs),
// bool +40/+41/+42, vint count + vector<LogicLong> +48.
// Split out of the entry wave; wire format unchanged.

#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

#include <vector>

namespace titan {

class AllianceTeamEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v4_);
        id8_.encode(s);
        s.writeVInt(v16_);
        encodeLogicLong(s, id24_);
        encodeLogicLong(s, id32_);
        s.writeBoolean(b40_);
        s.writeBoolean(b41_);
        s.writeBoolean(b42_);
        s.writeVInt(static_cast<i32>(members_.size()));
        for (const auto& m : members_) encodeLogicLong(s, m);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v4_ = s.readVInt();
        id8_ = LogicLong::decode(s);
        v16_ = s.readVInt();
        id24_.high = s.readVInt();
        id24_.low = s.readVInt();
        id32_.high = s.readVInt();
        id32_.low = s.readVInt();
        b40_ = s.readBoolean();
        b41_ = s.readBoolean();
        b42_ = s.readBoolean();
        const i32 n = s.readVInt();
        members_.clear();
        for (i32 i = 0; i < n; ++i) {
            LogicLong m;
            m.high = s.readVInt();
            m.low = s.readVInt();
            members_.push_back(m);
        }
    }
    i32 v0_ = 0, v4_ = 0, v16_ = 0;
    LogicLong id8_, id24_, id32_;
    bool b40_ = false, b41_ = false, b42_ = false;
    std::vector<LogicLong> members_;
};

} // namespace titan
