#pragma once

// EventData::encode @0x247168, decode @0x237294.
// 25-field daily-event descriptor; nested BattlePlayerMap/ChronosTextEntry/
// LogicGemOffer/LogicRankedSeason/ChronosFileEntry stay pending.
// Split out of the entry wave; wire format unchanged.

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"
#include "titan/messages/pending/BattlePlayerMap.hpp"
#include "titan/messages/pending/ChronosFileEntry.hpp"
#include "titan/messages/pending/ChronosTextEntry.hpp"
#include "titan/messages/pending/LogicGemOffer.hpp"
#include "titan/messages/pending/LogicRankedSeason.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class EventData : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;
    i32 v_[6] = {}; // +0..+20
    std::optional<DataReference> ref_; // +24
    i32 w32_ = 0, w36_ = 0; // +32/+36
    std::optional<std::string> name_; // +40
    i32 w48_ = 0, w52_ = 0, w56_ = 0; // +48/+52/+56
    std::vector<i32> vintLoop_; // +64 count + loop
    i32 w80_ = 0, w92_ = 0; // +80/+92
    std::unique_ptr<BattlePlayerMap> map_; // +120
    i32 w60_ = 0; // +60
    std::unique_ptr<LogicRankedSeason> season_; // +128 optional
    i32 w84_ = 0, w88_ = 0; // +84/+88
    std::unique_ptr<ChronosTextEntry> chrono1_, chrono2_; // +96/+104 optional
    std::unique_ptr<LogicGemOffer> offer_; // +136 optional
    std::vector<i32> optLoop_; // +72 vint-count (-1=null) + loop
    bool optLoopNull_ = false;
    std::unique_ptr<ChronosFileEntry> file_; // +112 optional
};

} // namespace titan
