#pragma once

// TeamEntry::encode @0x48ce6c, decode @0x7c1a40.
// Order below follows the binary exactly (counts + arrays):
//   vint +0, bool +4, vint +8, writeLong @+16, vint +24, bool +5/+6,
//   vint +28/+32, dataref @+40 (nullable),
//   writeBattlePlayerMap @0x8688d0 (bool + LogicBattlePlayerMap @+88, nullable),
//   TeamMemberEntry[] @+48, TeamInviteEntry[] @+56, TeamJoinRequest[] @+64,
//   int[] @+80, bool +72/+73/+74,
//   writeIntList @0x8bdf6c (nullable, null -> vint -1) @+104.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicBattlePlayerMap.hpp"
#include "titan/game/TeamInviteEntry.hpp"
#include "titan/game/TeamJoinRequest.hpp"
#include "titan/game/TeamMemberEntry.hpp"
#include "titan/messages/Nested.hpp"

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
