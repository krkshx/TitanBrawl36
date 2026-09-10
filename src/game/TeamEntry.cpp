// TeamEntry bodies — encode @0x48ce6c, decode @0x7c1a40.
// Order of reads/writes mirrors the binary exactly (see header).

#include "titan/game/TeamEntry.hpp"

namespace titan {

void TeamEntry::encode(ByteStream& s) const {
    s.writeVInt(v0_);
    s.writeBoolean(b4_);
    s.writeVInt(v8_);
    id16_.encode(s); // == writeLong
    s.writeVInt(v24_);
    s.writeBoolean(b5_);
    s.writeBoolean(b6_);
    s.writeVInt(v28_);
    s.writeVInt(v32_);
    DataReference::encodeNullable(s, ref40_);
    s.writeBoolean(battleMap_ != nullptr); // == writeBattlePlayerMap @0x8688d0
    if (battleMap_) battleMap_->encode(s);
    s.writeVInt(static_cast<i32>(members_.size()));
    for (const auto& e : members_) e->encode(s);
    s.writeVInt(static_cast<i32>(invites_.size()));
    for (const auto& e : invites_) e->encode(s);
    s.writeVInt(static_cast<i32>(joinRequests_.size()));
    for (const auto& e : joinRequests_) e->encode(s);
    s.writeVInt(static_cast<i32>(ints80_.size()));
    for (const i32 v : ints80_) s.writeVInt(v);
    s.writeBoolean(b72_);
    s.writeBoolean(b73_);
    s.writeBoolean(b74_);
    if (!intList_) { // == writeIntList @0x8bdf6c
        s.writeVInt(-1);
    } else {
        s.writeVInt(static_cast<i32>(intList_->size()));
        for (const i32 v : *intList_) s.writeVInt(v);
    }
}

void TeamEntry::decode(ByteStream& s) {
    v0_ = s.readVInt();
    b4_ = s.readBoolean();
    v8_ = s.readVInt();
    id16_ = LogicLong::decode(s);
    v24_ = s.readVInt();
    b5_ = s.readBoolean();
    b6_ = s.readBoolean();
    v28_ = s.readVInt();
    v32_ = s.readVInt();
    ref40_ = DataReference::decodeNullable(s);
    if (s.readBoolean()) { // == readBattlePlayerMap
        battleMap_ = std::make_unique<LogicBattlePlayerMap>();
        battleMap_->decode(s);
    } else {
        battleMap_.reset();
    }
    const i32 nMembers = s.readVInt();
    members_.clear();
    for (i32 i = 0; i < nMembers; ++i) {
        auto e = std::make_unique<TeamMemberEntry>();
        e->decode(s);
        members_.push_back(std::move(e));
    }
    const i32 nInvites = s.readVInt();
    invites_.clear();
    for (i32 i = 0; i < nInvites; ++i) {
        auto e = std::make_unique<TeamInviteEntry>();
        e->decode(s);
        invites_.push_back(std::move(e));
    }
    const i32 nJoin = s.readVInt();
    joinRequests_.clear();
    for (i32 i = 0; i < nJoin; ++i) {
        auto e = std::make_unique<TeamJoinRequest>();
        e->decode(s);
        joinRequests_.push_back(std::move(e));
    }
    const i32 n80 = s.readVInt();
    ints80_.clear();
    for (i32 i = 0; i < n80; ++i) ints80_.push_back(s.readVInt());
    b72_ = s.readBoolean();
    b73_ = s.readBoolean();
    b74_ = s.readBoolean();
    const i32 nList = s.readVInt(); // == readIntList
    if (nList < 0) {
        intList_.reset();
    } else {
        intList_.emplace();
        for (i32 i = 0; i < nList; ++i) intList_->push_back(s.readVInt());
    }
}

} // namespace titan
