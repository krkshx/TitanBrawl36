// LogicDailyData bodies — encode @0x6985dc, decode @0x4356a4.
// Order of reads/writes mirrors the binary exactly (see header).

#include "titan/game/LogicDailyData.hpp"

namespace titan {

void LogicDailyData::encode(ByteStream& s) const {
    for (const i32 v : head_) s.writeVInt(v);
    DataReference::encodeNullable(s, refA_);
    DataReference::encodeNullable(s, refB_);
    s.writeVInt(static_cast<i32>(ints15_.size()));
    for (const i32 v : ints15_) s.writeVInt(v);
    s.writeVInt(static_cast<i32>(refs19_.size()));
    for (const auto& r : refs19_) r.encode(s);
    s.writeVInt(static_cast<i32>(skins_.size()));
    for (const auto& [key, val] : skins_) {
        s.writeVInt(key);
        DataReference::encodeNullable(s, val);
    }
    s.writeVInt(static_cast<i32>(refs29_.size()));
    for (const auto& r : refs29_) r.encode(s);
    s.writeVInt(static_cast<i32>(refs33_.size()));
    for (const auto& r : refs33_) r.encode(s);
    s.writeVInt(static_cast<i32>(refs37_.size()));
    for (const auto& r : refs37_) r.encode(s);
    for (const i32 v : tail39_) s.writeVInt(v);
    s.writeBoolean(flag152_);
    for (const i32 v : tail43_) s.writeVInt(v);
    if (!forced_) throw pending_reverse("LogicDailyData needs ForcedDrops");
    forced_->encode(s);
    s.writeBoolean(offerA_ != nullptr);
    if (offerA_) offerA_->encode(s);
    s.writeBoolean(offerB_ != nullptr);
    if (offerB_) offerB_->encode(s);
    s.writeBoolean(flag216_);
    for (const i32 v : tail55_) s.writeVInt(v);
    s.writeVInt(static_cast<i32>(offers_.size()));
    for (const auto& o : offers_) o->encode(s);
    s.writeVInt(static_cast<i32>(ads_.size()));
    for (const auto& a : ads_) a->encode(s);
    s.writeVInt(v64_);
    s.writeVInt(v65_);
    s.writeVInt(static_cast<i32>(ints69_.size()));
    for (const i32 v : ints69_) s.writeVInt(v);
    s.writeVInt(v70_);
    s.writeVInt(v71_);
    DataReference::encodeNullable(s, ref288_);
    s.writeString(strA_.has_value() ? &strA_.value() : nullptr);
    s.writeString(strB_.has_value() ? &strB_.value() : nullptr);
    s.writeVInt(static_cast<i32>(intValues_.size()));
    for (const auto& e : intValues_) e->encode(s);
    s.writeVInt(static_cast<i32>(cooldowns_.size()));
    for (const auto& e : cooldowns_) e->encode(s);
    s.writeVInt(static_cast<i32>(brawlPass_.size()));
    for (const auto& e : brawlPass_) e->encode(s);
    s.writeVInt(static_cast<i32>(proLeague_.size()));
    for (const auto& e : proLeague_) e->encode(s);
    s.writeBoolean(quests_ != nullptr);
    if (quests_) quests_->encode(s);
    s.writeBoolean(vanity_ != nullptr);
    if (vanity_) vanity_->encode(s);
    s.writeBoolean(ranked_ != nullptr);
    if (ranked_) ranked_->encode(s);
    s.writeInt(tail102_);
}

void LogicDailyData::decode(ByteStream& s) {
    for (i32& v : head_) v = s.readVInt();
    refA_ = DataReference::decodeNullable(s);
    refB_ = DataReference::decodeNullable(s);
    const i32 n15 = s.readVInt();
    ints15_.clear();
    for (i32 i = 0; i < n15; ++i) ints15_.push_back(s.readVInt());
    const i32 n19 = s.readVInt();
    refs19_.clear();
    for (i32 i = 0; i < n19; ++i) {
        DataReference r;
        r.classId = s.readVInt();
        r.instanceId = s.readVInt();
        refs19_.push_back(r);
    }
    // NB: the binary reads the next key at the bottom of each iteration,
    // so the final key-read actually consumes the following count field.
    // The consumed byte sequence is identical to a plain pairs loop.
    const i32 nSkins = s.readVInt();
    skins_.clear();
    for (i32 i = 0; i < nSkins; ++i) {
        const i32 key = s.readVInt();
        skins_.emplace_back(key, DataReference::decodeNullable(s));
    }
    const i32 n29 = s.readVInt();
    refs29_.clear();
    for (i32 i = 0; i < n29; ++i) {
        DataReference r;
        r.classId = s.readVInt();
        r.instanceId = s.readVInt();
        refs29_.push_back(r);
    }
    const i32 n33 = s.readVInt();
    refs33_.clear();
    for (i32 i = 0; i < n33; ++i) {
        DataReference r;
        r.classId = s.readVInt();
        r.instanceId = s.readVInt();
        refs33_.push_back(r);
    }
    const i32 n37 = s.readVInt();
    refs37_.clear();
    for (i32 i = 0; i < n37; ++i) {
        DataReference r;
        r.classId = s.readVInt();
        r.instanceId = s.readVInt();
        refs37_.push_back(r);
    }
    for (i32& v : tail39_) v = s.readVInt();
    flag152_ = s.readBoolean();
    for (i32& v : tail43_) v = s.readVInt();
    forced_ = std::make_unique<ForcedDrops>();
    forced_->decode(s);
    if (s.readBoolean()) {
        offerA_ = std::make_unique<TimedOffer>();
        offerA_->decode(s);
    } else {
        offerA_.reset();
    }
    if (s.readBoolean()) {
        offerB_ = std::make_unique<TimedOffer>();
        offerB_->decode(s);
    } else {
        offerB_.reset();
    }
    flag216_ = s.readBoolean();
    for (i32& v : tail55_) v = s.readVInt();
    const i32 nOffers = s.readVInt();
    offers_.clear();
    for (i32 i = 0; i < nOffers; ++i) {
        auto o = std::make_unique<LogicOfferBundle>();
        o->decode(s);
        offers_.push_back(std::move(o));
    }
    const i32 nAds = s.readVInt();
    ads_.clear();
    for (i32 i = 0; i < nAds; ++i) {
        auto a = std::make_unique<AdStatus>();
        a->decode(s);
        ads_.push_back(std::move(a));
    }
    v64_ = s.readVInt();
    v65_ = s.readVInt();
    const i32 n69 = s.readVInt();
    ints69_.clear();
    for (i32 i = 0; i < n69; ++i) ints69_.push_back(s.readVInt());
    v70_ = s.readVInt();
    v71_ = s.readVInt();
    ref288_ = DataReference::decodeNullable(s);
    strA_ = s.readString();
    strB_ = s.readString();
    const i32 nIntValues = s.readVInt();
    intValues_.clear();
    for (i32 i = 0; i < nIntValues; ++i) {
        auto e = std::make_unique<IntValueEntry>();
        e->decode(s);
        intValues_.push_back(std::move(e));
    }
    const i32 nCooldowns = s.readVInt();
    cooldowns_.clear();
    for (i32 i = 0; i < nCooldowns; ++i) {
        auto e = std::make_unique<CooldownEntry>();
        e->decode(s);
        cooldowns_.push_back(std::move(e));
    }
    const i32 nBp = s.readVInt();
    brawlPass_.clear();
    for (i32 i = 0; i < nBp; ++i) {
        auto e = std::make_unique<BrawlPassSeasonData>();
        e->decode(s);
        brawlPass_.push_back(std::move(e));
    }
    const i32 nPl = s.readVInt();
    proLeague_.clear();
    for (i32 i = 0; i < nPl; ++i) {
        auto e = std::make_unique<ProLeagueSeasonData>();
        e->decode(s);
        proLeague_.push_back(std::move(e));
    }
    if (s.readBoolean()) {
        quests_ = std::make_unique<LogicQuests>();
        quests_->decode(s);
    } else {
        quests_.reset();
    }
    if (s.readBoolean()) {
        vanity_ = std::make_unique<VanityItems>();
        vanity_->decode(s);
    } else {
        vanity_.reset();
    }
    if (s.readBoolean()) {
        ranked_ = std::make_unique<LogicPlayerRankedSeasonData>();
        ranked_->decode(s);
    } else {
        ranked_.reset();
    }
    tail102_ = s.readInt();
}

} // namespace titan
