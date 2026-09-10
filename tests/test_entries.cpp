// Self-check for reversed nested entries and stream factories.

#include "titan/game/AddableFriendEntry.hpp"
#include "titan/game/AdStatus.hpp"
#include "titan/game/AllianceEventStreamEntry.hpp"
#include "titan/game/AllianceTeamEntry.hpp"
#include "titan/game/AvatarStreamEntry.hpp"
#include "titan/game/BattleLogEntry.hpp"
#include "titan/game/BrawlPassSeasonData.hpp"
#include "titan/game/CustomEvent.hpp"
#include "titan/game/EventSlot.hpp"
#include "titan/game/GatchaDrop.hpp"
#include "titan/game/HeroEntry.hpp"
#include "titan/game/LogicBitList.hpp"
#include "titan/game/LogicBattleEmotes.hpp"
#include "titan/game/LogicConfData.hpp"
#include "titan/game/LogicCondition.hpp"
#include "titan/game/LogicHeroUpgrades.hpp"
#include "titan/game/LogicPlayer.hpp"
#include "titan/game/LogicRewardConfig.hpp"
#include "titan/game/LogicPlayerRankedSeasonData.hpp"
#include "titan/game/ChatStreamEntry.hpp"
#include "titan/game/EventData.hpp"
#include "titan/game/FriendEntry.hpp"
#include "titan/game/CooldownEntry.hpp"
#include "titan/game/ChronosTextEntry.hpp"
#include "titan/game/ForcedDrops.hpp"
#include "titan/game/IntValueEntry.hpp"
#include "titan/game/JoinRequestAllianceStreamEntry.hpp"
#include "titan/game/LogicClientHome.hpp"
#include "titan/game/LogicDailyData.hpp"
#include "titan/game/LogicGemOffer.hpp"
#include "titan/game/LogicOfferBundle.hpp"
#include "titan/game/LogicQuests.hpp"
#include "titan/game/ProLeagueSeasonData.hpp"
#include "titan/game/QuestData.hpp"
#include "titan/game/ReleaseEntry.hpp"
#include "titan/game/TimedIntValueEntry.hpp"
#include "titan/game/VanityItemEntry.hpp"
#include "titan/game/VanityItemProp.hpp"
#include "titan/game/VanityItems.hpp"
#include "titan/game/TimedOffer.hpp"
#include "titan/game/LogicPlayerMap.hpp"
#include "titan/game/PlayerProfile.hpp"
#include "titan/game/StreamEntry.hpp"
#include "titan/game/StreamEntryFactory.hpp"
#include "titan/game/SuggestionEntry.hpp"

#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

template <typename T, typename Setup>
static void entryRoundTrip(Setup setup, T& back) {
    T out;
    setup(out);
    ByteStream s;
    out.encode(s);
    ByteStream d;
    d.setBuffer(s.data(), s.size());
    back.decode(d);
}

int main() {
    // StreamEntry base + alliance factory.
    {
        ChatStreamEntry back;
        entryRoundTrip<ChatStreamEntry>(
            [](ChatStreamEntry& e) {
                e.id1_ = LogicLong{1, 2};
                e.name_ = std::string("hi");
                e.text_ = std::string("yo");
            },
            back);
        CHECK(back.id1_.low == 2 && back.text_.value() == "yo");
    }
    {
        auto e = createAllianceStreamEntry(2);
        CHECK(e != nullptr);
        auto c = static_cast<ChatStreamEntry*>(e.get());
        (void)c;
        CHECK(createAllianceStreamEntry(7) == nullptr);
        CHECK(createAllianceStreamEntry(999) == nullptr);
        auto a = createAvatarStreamEntry(3);
        CHECK(a != nullptr);
    }
    // AllianceTeamEntry vint-pair logiclongs.
    {
        AllianceTeamEntry back;
        entryRoundTrip<AllianceTeamEntry>(
            [](AllianceTeamEntry& e) {
                e.v0_ = 1;
                e.id8_ = LogicLong{2, 3};
                e.id24_ = LogicLong{4, 5};
                e.b40_ = true;
                e.members_.push_back(LogicLong{6, 7});
            },
            back);
        CHECK(back.v0_ == 1 && back.id24_.high == 4 && back.b40_);
        CHECK(back.members_.size() == 1 && back.members_[0].low == 7);
    }
    // FriendEntry with club + display.
    {
        FriendEntry back;
        entryRoundTrip<FriendEntry>(
            [](FriendEntry& e) {
                e.id_ = LogicLong{9, 9};
                e.s1_ = std::string("n");
                e.ints_[0] = 42;
                e.hasClub_ = true;
                e.clubId_ = LogicLong{1, 1};
                e.clubName_ = std::string("club");
                e.hasDisplay_ = true;
                e.display_ = std::make_unique<PlayerDisplayData>();
                e.display_->name_ = std::string("d");
            },
            back);
        CHECK(back.s1_.value() == "n" && back.ints_[0] == 42);
        CHECK(back.hasClub_ && back.clubName_.value() == "club");
        CHECK(back.hasDisplay_ && back.display_->name_.value() == "d");
    }
    // SuggestionEntry wraps FriendEntry.
    {
        SuggestionEntry back;
        entryRoundTrip<SuggestionEntry>(
            [](SuggestionEntry& e) {
                e.v0_ = 5;
                e.entry_ = std::make_unique<FriendEntry>();
                e.entry_->s1_ = std::string("s");
            },
            back);
        CHECK(back.v0_ == 5 && back.entry_->s1_.value() == "s");
    }
    // LogicPlayerMap scalar core.
    {
        LogicPlayerMap back;
        entryRoundTrip<LogicPlayerMap>(
            [](LogicPlayerMap& m) {
                m.id_ = LogicLong{3, 3};
                m.name_ = std::string("map");
                m.data_ = std::make_unique<LogicCompressedString>();
                m.data_->setUncompressed("payload");
                m.q64_ = 123456789LL;
            },
            back);
        CHECK(back.name_.value() == "map" && back.q64_ == 123456789LL);
        CHECK(back.data_->getUncompressed() == std::vector<u8>({'p','a','y','l','o','a','d'}));
    }
    // EventData scalar core (BattlePlayerMap still pending -> loud throw).
    {
        EventData e;
        e.v_[0] = 11;
        e.map_ = std::make_unique<BattlePlayerMap>();
        ByteStream s;
        bool threw = false;
        try {
            e.encode(s);
        } catch (const pending_reverse&) {
            threw = true;
        }
        CHECK(threw);
    }
    // BattleLogEntry scalar core.
    {
        BattleLogEntry back;
        entryRoundTrip<BattleLogEntry>(
            [](BattleLogEntry& e) {
                e.v0_ = 1;
                e.q48_ = 99;
                e.hasOpt112_ = true;
                e.id112_ = LogicLong{4, 4};
                e.ref128_ = "r";
            },
            back);
        CHECK(back.v0_ == 1 && back.q48_ == 99);
        CHECK(back.hasOpt112_ && back.ref128_ == "r" && !back.hasOpt152_);
    }
    // AvatarStreamEntry shape.
    {
        AvatarStreamEntry e;
        e.b41_ = true;
        ByteStream s;
        e.encode(s);
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        AvatarStreamEntry back;
        back.decode(d);
        CHECK(back.b41_ && !back.b40_);
    }
    // JoinRequest + AllianceEvent entries.
    {
        JoinRequestAllianceStreamEntry back;
        entryRoundTrip<JoinRequestAllianceStreamEntry>(
            [](JoinRequestAllianceStreamEntry& e) { e.v56_ = 7; }, back);
        CHECK(back.v56_ == 7);
        AllianceEventStreamEntry back2;
        entryRoundTrip<AllianceEventStreamEntry>(
            [](AllianceEventStreamEntry& e) {
                e.v64_ = 1;
                e.hasId_ = true;
                e.id48_ = LogicLong{5, 5};
                e.s56_ = std::string("s");
            },
            back2);
        CHECK(back2.hasId_ && back2.s56_.value() == "s");
    }
    // LogicClientHome shell (pending nested throws loudly).
    {
        LogicClientHome h;
        bool threw = false;
        ByteStream s;
        try {
            h.encode(s);
        } catch (const pending_reverse&) {
            threw = true;
        }
        CHECK(threw);
    }
    // Wave-2 leaves (@0x664128, @0x354c54, @0x6b0790, @0x5402a0).
    {
        ForcedDrops back;
        entryRoundTrip<ForcedDrops>(
            [](ForcedDrops& e) {
                e.a_ = 3;
                e.b_ = 4;
                e.ids_ = {10, 20, 30};
            },
            back);
        CHECK(back.a_ == 3 && back.ids_.size() == 3 && back.ids_[2] == 30);
    }
    {
        TimedOffer back;
        entryRoundTrip<TimedOffer>(
            [](TimedOffer& e) {
                e.offer_ = DataReference{16, 5};
                e.v8_ = 100;
                e.v12_ = 200;
            },
            back);
        CHECK(back.offer_.has_value() && back.offer_->instanceId == 5);
        CHECK(back.v8_ == 100 && back.v12_ == 200);
    }
    {
        IntValueEntry back;
        entryRoundTrip<IntValueEntry>(
            [](IntValueEntry& e) {
                e.a_ = -7;
                e.b_ = 42;
            },
            back);
        CHECK(back.a_ == -7 && back.b_ == 42);
    }
    {
        CooldownEntry back;
        entryRoundTrip<CooldownEntry>(
            [](CooldownEntry& e) {
                e.id_ = 9;
                e.ref_ = DataReference{23, 1};
                e.value_ = 3600;
            },
            back);
        CHECK(back.id_ == 9 && back.value_ == 3600);
        CHECK(back.ref_.has_value() && back.ref_->classId == 23);
    }
    // LogicDailyData (@0x6985dc/@0x4356a4): populated round-trip.
    {
        LogicDailyData back;
        entryRoundTrip<LogicDailyData>(
            [](LogicDailyData& e) {
                e.head_[0] = 1;
                e.head_[6] = 6;
                e.refA_ = DataReference{16, 0};
                e.ints15_ = {1, 2};
                e.refs19_.push_back(DataReference{23, 3});
                e.skins_.emplace_back(5, DataReference{29, 0});
                e.skins_.emplace_back(7, std::nullopt);
                e.refs29_.push_back(DataReference{16, 1});
                e.tail39_[3] = 39;
                e.flag152_ = true;
                e.tail43_[0] = 43;
                e.forced_ = std::make_unique<ForcedDrops>();
                e.forced_->ids_ = {11};
                e.offerA_ = std::make_unique<TimedOffer>();
                e.offerA_->v8_ = 8;
                e.flag216_ = true;
                e.tail55_[4] = 55;
                e.v64_ = 64;
                e.ints69_ = {69};
                e.v70_ = 70;
                e.ref288_ = DataReference{16, 2};
                e.strA_ = std::string("hello");
                auto iv = std::make_unique<IntValueEntry>();
                iv->a_ = 1;
                iv->b_ = 2;
                e.intValues_.push_back(std::move(iv));
                auto cd = std::make_unique<CooldownEntry>();
                cd->id_ = 5;
                e.cooldowns_.push_back(std::move(cd));
                e.tail102_ = 102;
            },
            back);
        CHECK(back.head_[0] == 1 && back.head_[6] == 6);
        CHECK(back.refA_.has_value() && back.refA_->classId == 16);
        CHECK(!back.refB_.has_value());
        CHECK(back.ints15_.size() == 2 && back.ints15_[1] == 2);
        CHECK(back.refs19_.size() == 1 && back.refs19_[0].instanceId == 3);
        CHECK(back.skins_.size() == 2 && back.skins_[0].first == 5);
        CHECK(back.skins_[1].second.has_value() == false);
        CHECK(back.refs29_.size() == 1);
        CHECK(back.refs33_.empty() && back.refs37_.empty());
        CHECK(back.tail39_[3] == 39 && back.flag152_);
        CHECK(back.forced_ && back.forced_->ids_.size() == 1);
        CHECK(back.offerA_ && back.offerA_->v8_ == 8 && !back.offerB_);
        CHECK(back.flag216_ && back.tail55_[4] == 55);
        CHECK(back.offers_.empty() && back.ads_.empty());
        CHECK(back.v64_ == 64 && back.v65_ == 0);
        CHECK(back.ints69_.size() == 1 && back.v70_ == 70 && back.v71_ == 0);
        CHECK(back.ref288_.has_value() && back.ref288_->instanceId == 2);
        CHECK(back.strA_.has_value() && back.strA_.value() == "hello");
        CHECK(!back.strB_.has_value());
        CHECK(back.intValues_.size() == 1 && back.intValues_[0]->b_ == 2);
        CHECK(back.cooldowns_.size() == 1 && back.cooldowns_[0]->id_ == 5);
        CHECK(back.brawlPass_.empty() && back.proLeague_.empty());
        CHECK(!back.quests_ && !back.vanity_ && !back.ranked_);
        CHECK(back.tail102_ == 102);
    }
    // OfferBundle wave (@0x69b644 leaves @0x93ed54/@0x7b6d10).
    {
        LogicGemOffer back;
        entryRoundTrip<LogicGemOffer>(
            [](LogicGemOffer& e) {
                e.v0_ = 1;
                e.v1_ = 2;
                e.ref_ = DataReference{16, 7};
                e.v4_ = 4;
            },
            back);
        CHECK(back.v0_ == 1 && back.v4_ == 4);
        CHECK(back.ref_.has_value() && back.ref_->instanceId == 7);
    }
    {
        ChronosTextEntry back;
        entryRoundTrip<ChronosTextEntry>(
            [](ChronosTextEntry& e) {
                e.id_ = 77;
                e.text_ = std::string("sale!");
            },
            back);
        CHECK(back.id_ == 77 && back.text_ == "sale!");
    }
    {
        LogicOfferBundle back;
        entryRoundTrip<LogicOfferBundle>(
            [](LogicOfferBundle& e) {
                auto g = std::make_unique<LogicGemOffer>();
                g->v0_ = 5;
                g->v4_ = 50;
                e.gems_.push_back(std::move(g));
                e.v24_ = 24;
                e.v28_ = 28;
                e.v40_ = 40;
                e.b44_ = true;
                e.v52_ = 52;
                e.v80_ = 80;
                e.text_ = std::make_unique<ChronosTextEntry>();
                e.text_->id_ = 3;
                e.text_->text_ = std::string("bundle");
                e.b88_ = true;
                e.str_ = std::string("tag");
                e.v84_ = 84;
                e.v96_ = 96;
            },
            back);
        CHECK(back.gems_.size() == 1 && back.gems_[0]->v4_ == 50);
        CHECK(back.v24_ == 24 && back.v28_ == 28 && back.v40_ == 40);
        CHECK(back.b44_ && !back.b56_);
        CHECK(back.v52_ == 52 && back.v80_ == 80);
        CHECK(back.text_ && back.text_->text_ == "bundle");
        CHECK(back.b88_ && back.str_.has_value() && back.str_.value() == "tag");
        CHECK(back.v84_ == 84 && !back.b100_ && back.v92_ == 0 && back.v96_ == 96);
    }
    // AdStatus (@0x8d21fc).
    {
        AdStatus back;
        entryRoundTrip<AdStatus>(
            [](AdStatus& e) {
                e.a_ = 1;
                e.b_ = 2;
                e.c_ = 3;
            },
            back);
        CHECK(back.a_ == 1 && back.b_ == 2 && back.c_ == 3);
    }
    // Season/quests wave (@0x71b258, @0x5e8140, @0x1ae36c, @0x5980fc).
    {
        ProLeagueSeasonData back;
        entryRoundTrip<ProLeagueSeasonData>(
            [](ProLeagueSeasonData& e) {
                e.a_ = 11;
                e.b_ = 22;
            },
            back);
        CHECK(back.a_ == 11 && back.b_ == 22);
    }
    {
        BrawlPassSeasonData back;
        entryRoundTrip<BrawlPassSeasonData>(
            [](BrawlPassSeasonData& e) {
                e.v0_ = 1;
                e.v1_ = 2;
                e.b24_ = true;
                e.v28_ = 28;
                e.b32_ = true;
            },
            back);
        CHECK(back.v0_ == 1 && back.b24_ && back.v28_ == 28 && back.b32_);
        CHECK(!back.bits1_ && !back.bits2_);
    }
    {
        // Empty quest/vanity lists round-trip; non-empty need their entries.
        LogicQuests back;
        entryRoundTrip<LogicQuests>([](LogicQuests&) {}, back);
        CHECK(back.quests_.empty());
        VanityItems back2;
        entryRoundTrip<VanityItems>([](VanityItems&) {}, back2);
        CHECK(back2.items_.empty());
    }
    // BitList (@0x9740d8/@0x5c241c, C2 @0x7c7788) + users.
    {
        // Decode needs a preset size (binary builds C2(128) first).
        LogicBitList a(128);
        a.words()[0] = 0b1011;
        a.words()[3] = -1;
        ByteStream s;
        a.encode(s);
        CHECK(s.size() == 16); // 4 words, no count prefix
        LogicBitList b(128);
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        b.decode(d);
        CHECK(b.intCount() == 4);
        CHECK(b.words()[0] == 0b1011 && b.words()[3] == -1);
        CHECK(b.cachedBits() == 3 + 32);
    }
    {
        BrawlPassSeasonData back;
        entryRoundTrip<BrawlPassSeasonData>(
            [](BrawlPassSeasonData& e) {
                e.v0_ = 7;
                e.b24_ = true;
                e.v28_ = 9;
                e.bits1_ = std::make_unique<LogicBitList>(128);
                e.bits1_->words()[1] = 42;
            },
            back);
        CHECK(back.v0_ == 7 && back.b24_ && back.v28_ == 9 && !back.b32_);
        CHECK(back.bits1_ && back.bits1_->words()[1] == 42);
        CHECK(!back.bits2_);
    }
    {
        LogicPlayerRankedSeasonData back;
        entryRoundTrip<LogicPlayerRankedSeasonData>(
            [](LogicPlayerRankedSeasonData& e) {
                e.head_[0] = 5;
                e.head_[10] = 50;
                auto r = std::make_unique<LogicPlayerRewardData>();
                r->v16_ = 16;
                r->b20_ = true;
                e.rewards_.push_back(std::move(r));
            },
            back);
        CHECK(back.head_[0] == 5 && back.head_[10] == 50);
        CHECK(back.rewards_.size() == 1);
        CHECK(back.rewards_[0]->v16_ == 16 && back.rewards_[0]->b20_);
        CHECK(!back.rewards_[0]->config_);
    }
    // ConfData wave (@0x653b94/@0x6f6508 + leaves).
    {
        EventSlot back;
        entryRoundTrip<EventSlot>([](EventSlot& e) { e.v0_ = 13; }, back);
        CHECK(back.v0_ == 13);
    }
    {
        ReleaseEntry back;
        entryRoundTrip<ReleaseEntry>(
            [](ReleaseEntry& e) {
                e.ref_ = DataReference{16, 3};
                e.v8_ = 8;
                e.v12_ = 12;
            },
            back);
        CHECK(back.ref_.has_value() && back.v12_ == 12);
    }
    {
        TimedIntValueEntry back;
        entryRoundTrip<TimedIntValueEntry>(
            [](TimedIntValueEntry& e) {
                e.v_[0] = 1;
                e.v_[3] = 4;
            },
            back);
        CHECK(back.v_[0] == 1 && back.v_[3] == 4);
    }
    {
        CustomEvent back;
        entryRoundTrip<CustomEvent>(
            [](CustomEvent& e) {
                e.v0_ = 2;
                e.v1_ = 3;
                e.t0_ = std::make_unique<ChronosTextEntry>();
                e.t0_->text_ = std::string("a");
                e.t1_ = std::make_unique<ChronosTextEntry>();
                e.t2_ = std::make_unique<ChronosTextEntry>();
                e.t2_->id_ = 9;
            },
            back);
        CHECK(back.v0_ == 2 && back.t0_->text_ == "a" && back.t2_->id_ == 9);
    }
    {
        // QuestData (@0x467740) incl. the trailing v44 quirk.
        QuestData back;
        entryRoundTrip<QuestData>(
            [](QuestData& e) {
                e.v_[0] = 1;
                e.v_[9] = 9;
                e.b40_ = true;
                e.ref_ = DataReference{16, 4};
                e.v56_ = 56;
                e.v44_ = 44;
            },
            back);
        CHECK(back.v_[0] == 1 && back.v_[9] == 9 && back.b40_ && !back.b41_);
        CHECK(back.ref_.has_value() && back.v56_ == 56 && back.v44_ == 44);
    }
    {
        // Quests with a real entry inside (closes the DailyData loop).
        LogicQuests back;
        entryRoundTrip<LogicQuests>(
            [](LogicQuests& e) {
                auto q = std::make_unique<QuestData>();
                q->v_[1] = 11;
                q->v44_ = 1;
                e.quests_.push_back(std::move(q));
            },
            back);
        CHECK(back.quests_.size() == 1 && back.quests_[0]->v_[1] == 11);
    }
    {
        LogicConfData back;
        entryRoundTrip<LogicConfData>(
            [](LogicConfData& e) {
                e.v0_ = 1;
                auto sl = std::make_unique<EventSlot>();
                sl->v0_ = 2;
                e.slots_.push_back(std::move(sl));
                e.ints17_ = {17};
                e.ints21_ = {21, 22};
                e.b104_ = true;
                auto iv = std::make_unique<IntValueEntry>();
                iv->b_ = 5;
                e.intValues_.push_back(std::move(iv));
                auto tm = std::make_unique<TimedIntValueEntry>();
                tm->v_[2] = 7;
                e.timed_.push_back(std::move(tm));
                auto ce = std::make_unique<CustomEvent>();
                ce->t0_ = std::make_unique<ChronosTextEntry>();
                ce->t1_ = std::make_unique<ChronosTextEntry>();
                ce->t2_ = std::make_unique<ChronosTextEntry>();
                e.customs_.push_back(std::move(ce));
            },
            back);
        CHECK(back.v0_ == 1 && back.slots_.size() == 1 && back.slots_[0]->v0_ == 2);
        CHECK(back.events1_.empty() && back.events2_.empty());
        CHECK(back.ints17_.size() == 1 && back.ints21_.size() == 2 && back.ints25_.empty());
        CHECK(back.b104_ && back.releases_.empty());
        CHECK(back.intValues_.size() == 1 && back.intValues_[0]->b_ == 5);
        CHECK(back.timed_.size() == 1 && back.timed_[0]->v_[2] == 7);
        CHECK(back.customs_.size() == 1);
    }
    // Vanity/RewardConfig wave (@0x427c6c, @0x88d288/@0x710eb4 + leaves).
    {
        VanityItemProp back;
        entryRoundTrip<VanityItemProp>(
            [](VanityItemProp& e) {
                e.a_ = 3;
                e.b_ = 4;
            },
            back);
        CHECK(back.a_ == 3 && back.b_ == 4);
    }
    {
        LogicCondition back;
        entryRoundTrip<LogicCondition>(
            [](LogicCondition& e) {
                e.v8_ = 8;
                e.v12_ = 12;
            },
            back);
        CHECK(back.v8_ == 8 && back.v12_ == 12);
    }
    {
        VanityItemEntry back;
        entryRoundTrip<VanityItemEntry>(
            [](VanityItemEntry& e) {
                e.ref_ = DataReference{16, 9};
                auto p = std::make_unique<VanityItemProp>();
                p->a_ = 1;
                e.props_.push_back(std::move(p));
            },
            back);
        CHECK(back.ref_.has_value() && back.props_.size() == 1);
        CHECK(back.props_[0]->a_ == 1);
    }
    {
        // VanityItems with a real entry (closes another DailyData loop).
        VanityItems back;
        entryRoundTrip<VanityItems>(
            [](VanityItems& e) {
                auto en = std::make_unique<VanityItemEntry>();
                en->ref_ = DataReference{16, 1};
                e.items_.push_back(std::move(en));
            },
            back);
        CHECK(back.items_.size() == 1 && back.items_[0]->ref_.has_value());
    }
    {
        LogicRewardConfig back;
        entryRoundTrip<LogicRewardConfig>(
            [](LogicRewardConfig& e) {
                e.cond_ = std::make_unique<LogicCondition>();
                e.cond_->v8_ = 1;
                e.offer_ = std::make_unique<LogicGemOffer>();
                e.offer_->v4_ = 9;
            },
            back);
        CHECK(back.cond_ && back.cond_->v8_ == 1);
        CHECK(back.offer_ && back.offer_->v4_ == 9);
    }
    {
        // RewardData with a real config (closes the ranked loop).
        LogicPlayerRankedSeasonData back2;
        entryRoundTrip<LogicPlayerRankedSeasonData>(
            [](LogicPlayerRankedSeasonData& e) {
                auto r = std::make_unique<LogicPlayerRewardData>();
                r->config_ = std::make_unique<LogicRewardConfig>();
                r->config_->cond_ = std::make_unique<LogicCondition>();
                e.rewards_.push_back(std::move(r));
            },
            back2);
        CHECK(back2.rewards_.size() == 1 && back2.rewards_[0]->config_);
        CHECK(back2.rewards_[0]->config_->cond_ != nullptr);
        CHECK(!back2.rewards_[0]->config_->offer_);
    }
    // Player wave (@0x7509c0, @0x76f47c, @0x6bdfb0/@0x89bacc).
    {
        GatchaDrop back;
        entryRoundTrip<GatchaDrop>(
            [](GatchaDrop& e) {
                e.v4_ = 4;
                e.ref8_ = DataReference{16, 8};
                e.v0_ = 1;
                e.v40_ = 40;
                e.v44_ = 44;
            },
            back);
        CHECK(back.v4_ == 4 && back.v0_ == 1);
        CHECK(back.ref8_.has_value() && !back.ref16_);
        CHECK(back.v40_ == 40 && back.v44_ == 44);
    }
    {
        HeroEntry back;
        entryRoundTrip<HeroEntry>(
            [](HeroEntry& e) {
                e.ref0_ = DataReference{16, 0};
                e.v16_ = 16;
                e.v24_ = 24;
            },
            back);
        CHECK(back.ref0_.has_value() && !back.ref8_);
        CHECK(back.v16_ == 16 && back.v20_ == 0 && back.v24_ == 24);
    }
    {
        LogicPlayer back;
        entryRoundTrip<LogicPlayer>(
            [](LogicPlayer& e) {
                e.id_ = LogicLong{0, 123};
                e.v8_ = 8;
                e.v12_ = 12;
                e.v84_ = 84;
                e.v16_ = 16;
                e.ref24_ = DataReference{16, 2};
                e.display_ = std::make_unique<PlayerDisplayData>();
                e.b296_ = true;
            },
            back);
        CHECK(back.id_.low == 123);
        CHECK(back.v8_ == 8 && back.v84_ == 84 && back.v16_ == 16);
        CHECK(!back.ref56_ && back.ref24_.has_value());
        CHECK(!back.upgrades_ && !back.emotes_);
        CHECK(back.display_ && back.b296_);
    }
    // HeroUpgrades/Emotes (@0x5d26c8, @0x759184) + full player.
    {
        LogicHeroUpgrades back;
        entryRoundTrip<LogicHeroUpgrades>(
            [](LogicHeroUpgrades& e) {
                e.v0_ = 2;
                e.ref8_ = DataReference{16, 1};
            },
            back);
        CHECK(back.v0_ == 2 && back.ref8_.has_value() && !back.ref16_);
    }
    {
        LogicBattleEmotes back;
        entryRoundTrip<LogicBattleEmotes>(
            [](LogicBattleEmotes& e) {
                e.emotes_.push_back(DataReference{16, 5});
                e.emotes_.push_back(DataReference{16, 6});
            },
            back);
        CHECK(back.emotes_.size() == 2 && back.emotes_[1].instanceId == 6);
    }
    {
        // Player with optionals set (closes the avatar loop).
        LogicPlayer back;
        entryRoundTrip<LogicPlayer>(
            [](LogicPlayer& e) {
                e.display_ = std::make_unique<PlayerDisplayData>();
                e.upgrades_ = std::make_unique<LogicHeroUpgrades>();
                e.upgrades_->v0_ = 1;
                e.emotes_ = std::make_unique<LogicBattleEmotes>();
                e.emotes_->emotes_.push_back(DataReference{16, 0});
            },
            back);
        CHECK(back.upgrades_ && back.upgrades_->v0_ == 1);
        CHECK(back.emotes_ && back.emotes_->emotes_.size() == 1);
    }
    // LogicDailyData: encode is deterministic (byte-stable round-trip).
    {
        LogicDailyData a;
        a.head_[0] = 123456;
        a.refA_ = DataReference{16, 0};
        a.forced_ = std::make_unique<ForcedDrops>();
        a.strB_ = std::string("x");
        ByteStream s1;
        a.encode(s1);
        ByteStream d;
        d.setBuffer(s1.data(), s1.size());
        LogicDailyData b;
        b.decode(d);
        ByteStream s2;
        b.encode(s2);
        CHECK(s2.size() == s1.size());
        bool same = (s2.size() == s1.size());
        for (i32 i = 0; i < s1.size() && same; ++i) same = s1.data()[i] == s2.data()[i];
        CHECK(same);
    }

    if (failures == 0) std::puts("entries: all ok");
    return failures == 0 ? 0 : 1;
}
