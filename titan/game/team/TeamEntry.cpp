#pragma once

// TeamEntry::encode @0x48ce6c, decode @0x7c1a40.
// Order below follows the binary exactly (counts + arrays):
//   vint +0, bool +4, vint +8, writeLong @+16, vint +24, bool +5/+6,
//   vint +28/+32, dataref @+40 (nullable),
//   writeBattlePlayerMap @0x8688d0 (bool + LogicBattlePlayerMap @+88, nullable),
//   TeamMemberEntry[] @+48, TeamInviteEntry[] @+56, TeamJoinRequest[] @+64,
//   int[] @+80, bool +72/+73/+74,
//   writeIntList @0x8bdf6c (nullable, null -> vint -1) @+104.

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/game/battle/LogicBattlePlayerMap.cpp"
#include "titan/game/team/TeamInviteEntry.cpp"
#include "titan/game/team/TeamJoinRequest.cpp"
#include "titan/game/team/TeamMemberEntry.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <optional>
#include <vector>

namespace titan {

class TeamEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;

    i32 v0_ = 0; // +0
    bool b4_ = false, b5_ = false, b6_ = false; // +4/+5/+6
    i32 v8_ = 0; // +8
    LogicLong id16_; // +16 (writeLong)
    i32 v24_ = 0, v28_ = 0, v32_ = 0; // +24/+28/+32
    std::optional<DataReference> ref40_; // +40
    std::unique_ptr<LogicBattlePlayerMap> battleMap_; // +88 (nullable)
    std::vector<std::unique_ptr<TeamMemberEntry>> members_; // +48
    std::vector<std::unique_ptr<TeamInviteEntry>> invites_; // +56
    std::vector<std::unique_ptr<TeamJoinRequest>> joinRequests_; // +64
    std::vector<i32> ints80_; // +80
    bool b72_ = false, b73_ = false, b74_ = false; // +72/+73/+74
    std::optional<std::vector<i32>> intList_; // +104 (null -> -1)
};

} // namespace titan

// TeamEntry bodies — encode @0x48ce6c, decode @0x7c1a40.
// Order of reads/writes mirrors the binary exactly (see header).


namespace titan {

inline void TeamEntry::encode(ByteStream& s) const {
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

inline void TeamEntry::decode(ByteStream& s) {
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
