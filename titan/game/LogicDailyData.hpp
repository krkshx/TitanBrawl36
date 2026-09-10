#pragma once

// LogicDailyData::encode @0x6985dc, decode @0x4356a4.
// Field order below follows the binary exactly; member names carry the
// byte offset of the field in the binary object (same convention as
// LogicClientHome's v76_/b72_). Containers mirror the binary's
// LogicArrayList / LogicMapInt shapes (count vint + elements).
//
// Unresolved nested types stay as pending/* stubs (one class per file);
// setting them non-empty throws pending_reverse until their wave lands.

#include "titan/core/DataReference.hpp"
#include "titan/game/CooldownEntry.hpp"
#include "titan/game/ForcedDrops.hpp"
#include "titan/game/IntValueEntry.hpp"
#include "titan/game/TimedOffer.hpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/AdStatus.hpp"
#include "titan/game/BrawlPassSeasonData.hpp"
#include "titan/game/LogicOfferBundle.hpp"
#include "titan/game/LogicPlayerRankedSeasonData.hpp"
#include "titan/game/LogicQuests.hpp"
#include "titan/game/ProLeagueSeasonData.hpp"
#include "titan/game/VanityItems.hpp"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace titan {

class LogicDailyData : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;

    i32 head_[7] = {};                                        // +0..+24
    std::optional<DataReference> refA_;                       // +32
    std::optional<DataReference> refB_;                       // +40
    std::vector<i32> ints15_;                                 // +48 (+60 count)
    std::vector<DataReference> refs19_;                       // +64 (+76 count)
    // LogicMapInt<LogicSkinData const*> @+80: vint count + (vint key,
    // nullable skin dataref) pairs, keys in map order. +96 count.
    std::vector<std::pair<i32, std::optional<DataReference>>> skins_;
    std::vector<DataReference> refs29_;                       // +104 (+116 count)
    std::vector<DataReference> refs33_;                       // +120 (+132 count)
    std::vector<DataReference> refs37_;                       // +136 (+148 count)
    i32 tail39_[4] = {};                                      // +156..+168
    bool flag152_ = false;                                    // +152
    i32 tail43_[4] = {};                                      // +172..+184
    std::unique_ptr<ForcedDrops> forced_;                     // +192 (required)
    std::unique_ptr<TimedOffer> offerA_;                      // +200 (nullable)
    std::unique_ptr<TimedOffer> offerB_;                      // +208 (nullable)
    bool flag216_ = false;                                    // +216
    i32 tail55_[5] = {};                                      // +220..+236
    std::vector<std::unique_ptr<LogicOfferBundle>> offers_;   // +240
    std::vector<std::unique_ptr<AdStatus>> ads_;              // +248
    i32 v64_ = 0, v65_ = 0;                                   // +256, +260
    std::vector<i32> ints69_;                                 // +264 (+276 count)
    i32 v70_ = 0, v71_ = 0;                                   // +280, +284
    std::optional<DataReference> ref288_;                     // +288
    std::optional<std::string> strA_;                         // +304 String
    std::optional<std::string> strB_;                         // +312 String
    std::vector<std::unique_ptr<IntValueEntry>> intValues_;   // +320 (+332 count)
    std::vector<std::unique_ptr<CooldownEntry>> cooldowns_;   // +336 (+348 count)
    std::vector<std::unique_ptr<BrawlPassSeasonData>> brawlPass_; // +352 (+364)
    std::vector<std::unique_ptr<ProLeagueSeasonData>> proLeague_; // +368 (+380)
    std::unique_ptr<LogicQuests> quests_;                     // +384 (nullable)
    std::unique_ptr<VanityItems> vanity_;                     // +392 (nullable)
    std::unique_ptr<LogicPlayerRankedSeasonData> ranked_;     // +400 (nullable)
    i32 tail102_ = 0;                                         // +408
};

} // namespace titan
