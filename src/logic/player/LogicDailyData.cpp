#pragma once
#include "../offer/ForcedDrops.cpp"
#include "../offer/TimedOffer.cpp"
#include "../offer/LogicOfferBundle.cpp"
#include "../offer/AdStatus.cpp"
#include "../data/IntValueEntry.cpp"
#include "../data/CooldownEntry.cpp"
#include "../ranked/BrawlPassSeasonData.cpp"
#include "../ranked/ProLeagueSeasonData.cpp"
#include "../quest/LogicQuests.cpp"
#include "../vanity/VanityItems.cpp"
#include "../ranked/LogicPlayerRankedSeasonData.cpp"
#include <cstdint>
#include <string>
#include <vector>

struct DataRef {
    std::int32_t type = 0;
    std::int32_t id = 0;
};

class LogicDailyData {
public:
    void encode(ByteStream &s) const {
        s.writeVInt(a0_);
        s.writeVInt(a1_);
        s.writeVInt(a2_);
        s.writeVInt(a3_);
        s.writeVInt(a4_);
        s.writeVInt(a5_);
        s.writeVInt(a6_);
        ByteStreamHelper::writeDataReference(&s, r0_.type, r0_.id);
        ByteStreamHelper::writeDataReference(&s, r1_.type, r1_.id);
        s.writeVInt(static_cast<std::int32_t>(ids_.size()));
        for (std::int32_t v : ids_) {
            s.writeVInt(v);
        }
        s.writeVInt(static_cast<std::int32_t>(refs_.size()));
        for (std::int32_t r : refs_) {
            ByteStreamHelper::writeDataReference(&s, r);
        }
        s.writeVInt(cooldownSeconds_);
        s.writeVInt(brawlPassPoints_);
        s.writeVInt(starPoints_);
        s.writeVInt(eventPoints_);
        s.writeBoolean(bonusAvailable_);
        s.writeVInt(coins_);
        s.writeVInt(gems_);
        s.writeVInt(starpowerPoints_);
        s.writeVInt(tickets_);
    }
    void decode(ByteStream &s) {
        a0_ = s.readVInt();
        a1_ = s.readVInt();
        a2_ = s.readVInt();
        a3_ = s.readVInt();
        a4_ = s.readVInt();
        a5_ = s.readVInt();
        a6_ = s.readVInt();
        r0_.type = 0;
        r0_.id = 0;
        {
            std::int32_t cls = 0, inst = 0;
            ByteStreamHelper::readDataReference(&s, &cls, &inst);
            r0_.type = cls;
            r0_.id = inst;
        }
        {
            std::int32_t cls = 0, inst = 0;
            ByteStreamHelper::readDataReference(&s, &cls, &inst);
            r1_.type = cls;
            r1_.id = inst;
        }
        {
            std::int32_t n = s.readVInt();
            ids_.resize(static_cast<std::size_t>(n));
            for (std::int32_t i = 0; i < n; i++) {
                ids_[static_cast<std::size_t>(i)] = s.readVInt();
            }
        }
        {
            std::int32_t m = s.readVInt();
            refs_.resize(static_cast<std::size_t>(m));
            for (std::int32_t i = 0; i < m; i++) {
                refs_[static_cast<std::size_t>(i)] = ByteStreamHelper::readDataReference(&s);
            }
        }
        {
            std::int32_t m = s.readVInt();
            for (std::int32_t i = 0; i < m; i++) {
                refsTree_.push_back(ByteStreamHelper::readDataReference(&s));
            }
        }
        {
            std::int32_t m = s.readVInt();
            for (std::int32_t i = 0; i < m; i++) {
                refs2_.push_back(ByteStreamHelper::readDataReference(&s));
            }
        }
        {
            std::int32_t m = s.readVInt();
            for (std::int32_t i = 0; i < m; i++) {
                refs3_.push_back(ByteStreamHelper::readDataReference(&s));
            }
        }
        for (int i = 0; i < 4; i++) {
            u0_[i] = s.readVInt();
        }
        flag0_ = s.readBoolean();
        for (int i = 0; i < 4; i++) {
            u1_[i] = s.readVInt();
        }
        forced_ = ForcedDrops::decode(s);
        hasOffer0_ = s.readBoolean();
        if (hasOffer0_) {
            offer0_ = TimedOffer::decode(s);
        }
        hasOffer1_ = s.readBoolean();
        if (hasOffer1_) {
            offer1_ = TimedOffer::decode(s);
        }
        flag216_ = s.readBoolean();
        for (int i = 0; i < 5; i++) {
            extra5_[i] = s.readVInt();
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                bundles_.push_back(LogicOfferBundle::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                adStatuses_.push_back(AdStatus::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                intValues_.push_back(IntValueEntry::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                cooldowns_.push_back(CooldownEntry::decode(s));
            }
        }
        brawlPass_ = BrawlPassSeasonData::decode(s);
        proLeague_ = ProLeagueSeasonData::decode(s);
        hasQuests_ = s.readBoolean();
        if (hasQuests_) {
            quests_ = LogicQuests::decode(s);
        }
        hasVanity_ = s.readBoolean();
        if (hasVanity_) {
            vanity_ = VanityItems::decode(s);
        }
        hasRanked_ = s.readBoolean();
        if (hasRanked_) {
            ranked_ = LogicPlayerRankedSeasonData::decode(s);
        }
        tail_ = s.readVInt();
        ok_ = true;
    }
    std::int32_t a0_ = 0;
    std::int32_t a1_ = 0;
    std::int32_t a2_ = 0;
    std::int32_t a3_ = 0;
    std::int32_t a4_ = 0;
    std::int32_t a5_ = 0;
    std::int32_t a6_ = 0;
    DataRef r0_;
    DataRef r1_;
    std::vector<std::int32_t> ids_;
    std::vector<std::int32_t> refs_;
    std::vector<std::int32_t> refsTree_;
    std::vector<std::int32_t> refs2_;
    std::vector<std::int32_t> refs3_;
    std::int32_t u0_[4] = {0, 0, 0, 0};
    bool flag0_ = false;
    std::int32_t u1_[4] = {0, 0, 0, 0};
    ForcedDrops forced_;
    bool hasOffer0_ = false;
    TimedOffer offer0_;
    bool hasOffer1_ = false;
    TimedOffer offer1_;
    bool flag216_ = false;
    std::int32_t extra5_[5] = {0, 0, 0, 0, 0};
    std::vector<LogicOfferBundle> bundles_;
    std::vector<AdStatus> adStatuses_;
    std::vector<IntValueEntry> intValues_;
    std::vector<CooldownEntry> cooldowns_;
    BrawlPassSeasonData brawlPass_;
    ProLeagueSeasonData proLeague_;
    bool hasQuests_ = false;
    LogicQuests quests_;
    bool hasVanity_ = false;
    VanityItems vanity_;
    bool hasRanked_ = false;
    LogicPlayerRankedSeasonData ranked_;
    std::int32_t tail_ = 0;
    // Совместимость со старым использованием в тестах.
    std::int32_t cooldownSeconds_ = 0;
    std::int32_t brawlPassPoints_ = 0;
    std::int32_t starPoints_ = 0;
    std::int32_t eventPoints_ = 0;
    bool bonusAvailable_ = false;
    std::int32_t coins_ = 0;
    std::int32_t gems_ = 0;
    std::int32_t starpowerPoints_ = 0;
    std::int32_t tickets_ = 0;
    bool ok_ = false;
};
