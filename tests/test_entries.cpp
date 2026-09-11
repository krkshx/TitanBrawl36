// Self-check for reversed nested entries and stream factories.

#include "titan/game/social/AddableFriendEntry.cpp"
#include "titan/game/util/AdStatus.cpp"
#include "titan/game/alliance/AllianceWarFaction.cpp"
#include "titan/game/alliance/AllianceWarNode.cpp"
#include "titan/game/home/ChronosFileEntry.cpp"
#include "titan/game/alliance/AllianceEventStreamEntry.cpp"
#include "titan/game/alliance/AllianceTeamEntry.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"
#include "titan/game/stream/BattleReportStreamEntry.cpp"
#include "titan/game/battle/BattleLogEntry.cpp"
#include "titan/game/battle/BattleLogPlayerEntry.cpp"
#include "titan/game/home/BrawlPassSeasonData.cpp"
#include "titan/game/social/BrawlTvChannelInfo.cpp"
#include "titan/game/util/CustomEvent.cpp"
#include "titan/game/shop/DeliveryUnit.cpp"
#include "titan/game/home/EventSlot.cpp"
#include "titan/game/social/FriendOnlineStatusEntry.cpp"
#include "titan/game/home/Home.cpp"
#include "titan/messages/pending/FriendOnlineStatus.cpp" // alias check
#include "titan/game/avatar/HeroDataEntry.cpp"
#include "titan/game/avatar/HeroEntry.cpp"
#include "titan/game/util/LogicBitList.cpp"
#include "titan/game/util/LatencyData.cpp"
#include "titan/game/util/LatencyTestConfiguration.cpp"
#include "titan/game/battle/LogicBattleEmotes.cpp"
#include "titan/game/data/LogicConfData.cpp"
#include "titan/game/util/LogicCondition.cpp"
#include "titan/game/avatar/LogicHeroUpgrades.cpp"
#include "titan/game/home/LogicMilestoneProgress.cpp"
#include "titan/game/player/LogicPlayer.cpp"
#include "titan/game/battle/LogicRankedMatch.cpp"
#include "titan/game/battle/LogicRankedMatchPlayer.cpp"
#include "titan/game/battle/LogicRankedMatchRoundState.cpp"
#include "titan/game/battle/LogicRankedMatchResultDebugInfo.cpp"
#include "titan/game/player/LogicRankedSeason.cpp"
#include "titan/game/player/LogicRankRewardConfig.cpp"
#include "titan/game/notif/BandNotification.cpp"
#include "titan/game/notif/BoxRewardNotification.cpp"
#include "titan/game/notif/DonateNotification.cpp"
#include "titan/game/notif/GemRewardNotification.cpp"
#include "titan/game/notif/RankedMidSeasonRewardNotification.cpp"
#include "titan/game/notif/RankedSeasonEndNotification.cpp"
#include "titan/game/notif/RevokeNotification.cpp"
#include "titan/game/player/ScoreEntry.cpp"
#include "titan/game/notif/SkinPurchaseOptionRewardNotification.cpp"
#include "titan/game/notif/StarPointsNotification.cpp"
#include "titan/game/team/TeamEntry.cpp"
#include "titan/game/team/TeamInviteEntry.cpp"
#include "titan/game/team/TeamInvitation.cpp"
#include "titan/game/team/TeamInvitationDataEntry.cpp"
#include "titan/game/team/TeamJoinRequest.cpp"
#include "titan/game/team/TeamMemberEntry.cpp"
#include "titan/game/player/LogicRewardConfig.cpp"
#include "titan/game/player/LogicRewards.cpp"
#include "titan/game/util/LogicTencentAntiAddictionInstruction.cpp"
#include "titan/game/util/LogicUuid.cpp"
#include "titan/game/util/LogicVector2.cpp"
#include "titan/game/avatar/PlayerEntry.cpp"
#include "titan/game/battle/PlayAgainStatus.cpp"
#include "titan/game/player/PlayerRankingData.cpp"
#include "titan/game/alliance/AllianceRankingData.cpp"
#include "titan/game/player/RankingEntry.cpp"
#include "titan/game/player/LogicPlayerRankedSeasonData.cpp"
#include "titan/game/stream/ChatStreamEntry.cpp"
#include "titan/game/stream/MessageDataStreamEntry.cpp"
#include "titan/game/stream/QuickChatStreamEntry.cpp"
#include "titan/game/stream/ReplayStreamEntry.cpp"
#include "titan/game/stream/TeamCreatedStreamEntry.cpp"
#include "titan/game/home/EventData.cpp"
#include "titan/game/social/FriendEntry.cpp"
#include "titan/game/player/CooldownEntry.cpp"
#include "titan/game/home/ChronosTextEntry.cpp"
#include "titan/game/player/ForcedDrops.cpp"
#include "titan/game/player/IntValueEntry.cpp"
#include "titan/game/alliance/JoinRequestAllianceStreamEntry.cpp"
#include "titan/game/alliance/JoinAllianceResponseAvatarStreamEntry.cpp"
#include "titan/game/home/Home.cpp"
#include "titan/game/player/LogicDailyData.cpp"
#include "titan/game/shop/LogicGemOffer.cpp"
#include "titan/game/shop/LogicOfferBundle.cpp"
#include "titan/game/home/LogicQuests.cpp"
#include "titan/game/battle/LobbyInfoEntry.cpp"
#include "titan/game/notif/NotificationFactory.cpp"
#include "titan/game/home/ProLeagueSeasonData.cpp"
#include "titan/game/home/QuestData.cpp"
#include "titan/game/util/ReleaseEntry.cpp"
#include "titan/game/player/RewardEntry.cpp"
#include "titan/game/player/TimedIntValueEntry.cpp"
#include "titan/game/avatar/VanityItemEntry.cpp"
#include "titan/game/avatar/VanityItemProp.cpp"
#include "titan/game/player/XpEntry.cpp"
#include "titan/game/data/LogicData.cpp"
#include "titan/game/avatar/VanityItems.cpp"
#include "titan/game/shop/TimedOffer.cpp"
#include "titan/game/player/LogicPlayerMap.cpp"
#include "titan/game/player/PlayerProfile.cpp"
#include "titan/game/util/StatusChangeEntry.cpp"
#include "titan/game/stream/StreamEntry.cpp"
#include "titan/game/stream/StreamEntryFactory.cpp"
#include "titan/game/social/SuggestionEntry.cpp"

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
        MessageDataStreamEntry back;
        entryRoundTrip<MessageDataStreamEntry>(
            [](MessageDataStreamEntry& e) {
                e.id2_ = LogicLong{3, 4};
                e.ref48_.classId = 1;
                e.ref48_.instanceId = 7;
            },
            back);
        CHECK(back.id2_.low == 4 && back.ref48_.classId == 1 &&
              back.ref48_.instanceId == 7);
        CHECK(back.entryType() == 6);
        CHECK(createAllianceStreamEntry(6) != nullptr);
    }
    {
        QuickChatStreamEntry back;
        entryRoundTrip<QuickChatStreamEntry>(
            [](QuickChatStreamEntry& e) {
                e.ref48_.classId = 2;
                e.ref48_.instanceId = 9;
                e.text56_ = std::string("gl hf");
                e.v64_ = 11;
                e.v72_ = 22;
                e.v76_ = 33;
            },
            back);
        CHECK(back.ref48_.instanceId == 9 && back.text56_.value() == "gl hf" &&
              back.v64_ == 11 && back.v72_ == 22 && back.v76_ == 33);
        CHECK(back.entryType() == 8);
        CHECK(createAllianceStreamEntry(8) != nullptr);
    }
    {
        QuickChatStreamEntry back;
        entryRoundTrip<QuickChatStreamEntry>(
            [](QuickChatStreamEntry& e) { e.v64_ = 5; }, back);
        CHECK(!back.text56_.has_value() && back.v64_ == 5);
    }
    {
        ReplayStreamEntry back;
        entryRoundTrip<ReplayStreamEntry>(
            [](ReplayStreamEntry& e) {
                e.v44_ = 1;
                e.name48_ = std::string("replay");
                e.b56_ = true;
                e.v72_ = 7;
                e.v64_ = 8;
                e.id80_ = "custom-id";
                e.v104_ = 9;
            },
            back);
        CHECK(back.v44_ == 1 && back.name48_.value() == "replay" &&
              back.b56_ && back.v72_ == 7 && back.v64_ == 8 &&
              back.id80_ == "custom-id" && back.v104_ == 9);
        CHECK(back.entryType() == 5);
        CHECK(createAllianceStreamEntry(5) != nullptr);
    }
    {
        // Fresh objects carry the binary's template replay id.
        ReplayStreamEntry fresh;
        CHECK(fresh.id80_ ==
              "44838203_a45f_46c9_9ec2_b0f70bb8a77f_12000");
    }
    {
        TeamCreatedStreamEntry back;
        entryRoundTrip<TeamCreatedStreamEntry>(
            [](TeamCreatedStreamEntry& e) {
                e.name48_ = std::string("team-up");
                e.v56_ = 1;
                e.v60_ = 2;
                e.v64_ = 3;
            },
            back);
        CHECK(back.name48_.value() == "team-up" && back.v56_ == 1 &&
              back.v60_ == 2 && back.v64_ == 3);
        CHECK(back.entryType() == 77);
        CHECK(createAllianceStreamEntry(77) != nullptr);
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
        auto b1 = createAvatarStreamEntry(1);
        auto b2 = createAvatarStreamEntry(2);
        CHECK(b1 != nullptr && b2 != nullptr);
        CHECK(static_cast<BattleReportStreamEntry*>(b1.get())->entryType() == 1);
        CHECK(static_cast<BattleReportStreamEntry*>(b2.get())->entryType() == 2);
    }
    {
        BattleReportStreamEntry back(2);
        entryRoundTrip<BattleReportStreamEntry>(
            [](BattleReportStreamEntry& e) {
                e.v48_ = 4;
                e.b56_ = true;
                e.v72_ = 5;
                e.v76_ = 6;
                e.v80_ = 7;
                e.v60_ = 8;
                e.text64_ = std::string("mvp");
            },
            back);
        CHECK(back.v48_ == 4 && back.b56_ && back.v80_ == 7 &&
              back.v60_ == 8 && back.text64_.value() == "mvp");
        CHECK(back.entryType() == 2);
    }
    {
        // Null string: v60 stays at the ctor default (-1), nothing extra
        // hits the wire.
        BattleReportStreamEntry back;
        entryRoundTrip<BattleReportStreamEntry>(
            [](BattleReportStreamEntry& e) { e.v48_ = 3; }, back);
        CHECK(!back.text64_.has_value() && back.v60_ == -1 &&
              back.v48_ == 3);
        CHECK(back.entryType() == 1);
    }
    {
        JoinAllianceResponseAvatarStreamEntry back;
        entryRoundTrip<JoinAllianceResponseAvatarStreamEntry>(
            [](JoinAllianceResponseAvatarStreamEntry& e) {
                e.name48_ = std::string("resp");
                e.v56_ = 1;
                e.ref64_.classId = 3;
                e.ref64_.instanceId = 4;
                e.v88_ = 2;
                e.b72_ = true;
                e.text80_ = std::string("welcome");
            },
            back);
        CHECK(back.name48_.value() == "resp" && back.v56_ == 1 &&
              back.ref64_.instanceId == 4 && back.v88_ == 2 && back.b72_ &&
              back.text80_.value() == "welcome");
        CHECK(back.entryType() == 3);
        auto e3 = createAvatarStreamEntry(3);
        CHECK(e3 != nullptr);
        CHECK(static_cast<JoinAllianceResponseAvatarStreamEntry*>(e3.get())
                  ->entryType() == 3);
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
    {
        // VanityItems::has/add (@0x448ce8/@0x8a55b4, identity semantics).
        VanityItems items;
        LogicData emote(nullptr, 14, 0);
        CHECK(!items.has(emote));
        items.add(emote);
        CHECK(items.has(emote));
        LogicData other(nullptr, 14, 1);
        CHECK(!items.has(other));
        items.add(emote); // duplicate -> no-op
        CHECK(items.items_.size() == 1);
        CHECK(items.items_[0]->props_.size() == 1);
        CHECK(items.items_[0]->props_[0]->a_ == 1);
        CHECK(items.items_[0]->props_[0]->b_ == 1);
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
    // Small leaves (@0x72d8ec, @0x4481e8, @0x6b920c, @0x246da0).
    {
        HeroDataEntry back;
        entryRoundTrip<HeroDataEntry>(
            [](HeroDataEntry& e) {
                e.ref0_ = DataReference{16, 0};
                e.v16_ = 3;
                e.b20_ = true;
                e.str_ = std::string("shelly");
            },
            back);
        CHECK(back.ref0_.has_value() && !back.ref8_);
        CHECK(back.v16_ == 3 && back.b20_ && back.str_.value() == "shelly");
    }
    {
        DeliveryUnit back;
        entryRoundTrip<DeliveryUnit>(
            [](DeliveryUnit& e) {
                e.v8_ = 8;
                auto d = std::make_unique<GatchaDrop>();
                d->v0_ = 1;
                e.drops_.push_back(std::move(d));
            },
            back);
        CHECK(back.v8_ == 8 && back.drops_.size() == 1);
        CHECK(back.drops_[0]->v0_ == 1);
    }
    {
        LogicMilestoneProgress back;
        entryRoundTrip<LogicMilestoneProgress>(
            [](LogicMilestoneProgress& e) { e.v_[2] = 9; }, back);
        CHECK(back.v_[0] == 0 && back.v_[2] == 9);
    }
    {
        AllianceWarFaction back;
        entryRoundTrip<AllianceWarFaction>(
            [](AllianceWarFaction& e) {
                e.a_ = 1;
                e.b_ = 2;
            },
            back);
        CHECK(back.a_ == 1 && back.b_ == 2);
    }
    // War/misc leaves (@0x5f7f9c, @0x29fda4, @0x67ddd4).
    {
        AllianceWarNode back;
        entryRoundTrip<AllianceWarNode>(
            [](AllianceWarNode& e) {
                e.v0_ = 1;
                e.v12_ = 12;
                e.ref_ = DataReference{16, 2};
                e.v32_ = 32;
                e.ids_ = {7, 8};
            },
            back);
        CHECK(back.v0_ == 1 && back.v12_ == 12 && back.v32_ == 32);
        CHECK(back.ref_.has_value() && back.ids_.size() == 2 && back.ids_[1] == 8);
    }
    {
        LogicVector2 back;
        entryRoundTrip<LogicVector2>(
            [](LogicVector2& e) {
                e.x_ = -100;
                e.y_ = 250;
            },
            back);
        CHECK(back.x_ == -100 && back.y_ == 250);
    }
    {
        XpEntry back;
        entryRoundTrip<XpEntry>(
            [](XpEntry& e) {
                e.a_ = 5;
                e.b_ = 6;
            },
            back);
        CHECK(back.a_ == 5 && back.b_ == 6);
    }
    // LogicUuid (@0x29dc9c, exercises writeVLong/readVLong).
    {
        LogicUuid back;
        entryRoundTrip<LogicUuid>(
            [](LogicUuid& e) {
                e.hi_ = 0x0123456789ABCDEFULL;
                e.lo_ = 0xFEDCBA9876543210ULL;
            },
            back);
        CHECK(back.hi_ == 0x0123456789ABCDEFULL);
        CHECK(back.lo_ == 0xFEDCBA9876543210ULL);
    }
    // PlayerEntry (@0x7b5e20/@0x575470, writeLong arg verified in asm).
    {
        PlayerEntry back;
        entryRoundTrip<PlayerEntry>(
            [](PlayerEntry& e) {
                e.b0_ = true;
                e.b2_ = true;
                e.ref8_ = DataReference{16, 1};
                e.v24_ = 24;
                e.v36_ = 36;
                e.opt40_ = LogicLong{7, 9};
                e.display_ = std::make_unique<PlayerDisplayData>();
            },
            back);
        CHECK(back.b0_ && !back.b1_ && back.b2_);
        CHECK(back.ref8_.has_value() && !back.ref16_);
        CHECK(back.v24_ == 24 && back.v28_ == 0 && back.v36_ == 36);
        CHECK(back.opt40_.has_value() && back.opt40_->high == 7 && back.opt40_->low == 9);
        CHECK(back.display_ != nullptr);
    }
    // RankedSeason + TeamEntry wave (@0x2a66b0, @0x48ce6c/@0x7c1a40).
    {
        LogicRankRewardConfig back;
        entryRoundTrip<LogicRankRewardConfig>(
            [](LogicRankRewardConfig& e) {
                e.v8_ = 8;
                e.v12_ = 12;
            },
            back);
        CHECK(back.v8_ == 8 && back.v12_ == 12);
    }
    {
        LogicRankedSeason back;
        entryRoundTrip<LogicRankedSeason>(
            [](LogicRankedSeason& e) {
                e.v8_ = 1;
                e.name_ = std::string("season7");
                e.v28_ = 28;
                auto r = std::make_unique<LogicRewardConfig>();
                e.rewards_.push_back(std::move(r));
                auto rr = std::make_unique<LogicRankRewardConfig>();
                rr->v12_ = 3;
                e.rankRewards_.push_back(std::move(rr));
            },
            back);
        CHECK(back.v8_ == 1 && back.name_.value() == "season7");
        CHECK(back.v24_ == 0 && back.v28_ == 28);
        CHECK(back.rewards_.size() == 1 && back.rankRewards_.size() == 1);
        CHECK(back.rankRewards_[0]->v12_ == 3);
    }
    {
        TeamInviteEntry back;
        entryRoundTrip<TeamInviteEntry>(
            [](TeamInviteEntry& e) {
                e.id0_ = LogicLong{1, 2};
                e.name_ = std::string("inv");
                e.v28_ = 5;
            },
            back);
        CHECK(back.id0_.low == 2 && back.name_.value() == "inv" && back.v28_ == 5);
    }
    {
        TeamJoinRequest back;
        entryRoundTrip<TeamJoinRequest>(
            [](TeamJoinRequest& e) {
                e.id8_ = LogicLong{3, 4};
                e.friend_ = std::make_unique<FriendEntry>();
                e.friend_->v80_ = 6;
            },
            back);
        CHECK(back.id8_.low == 4 && back.friend_ && back.friend_->v80_ == 6);
    }
    {
        TeamMemberEntry back;
        entryRoundTrip<TeamMemberEntry>(
            [](TeamMemberEntry& e) {
                e.b0_ = true;
                e.id8_ = LogicLong{0, 77};
                e.ref16_ = DataReference{16, 1};
                e.v32_ = 32;
                e.v64_ = 64;
                e.display_ = std::make_unique<PlayerDisplayData>();
                e.v96_ = 96;
            },
            back);
        CHECK(back.b0_ && !back.b1_);
        CHECK(back.id8_.low == 77 && back.ref16_.has_value() && !back.ref24_);
        CHECK(back.v32_ == 32 && back.v64_ == 64 && back.v96_ == 96);
        CHECK(back.display_ && !back.ref80_ && !back.ref88_);
    }
    {
        // TeamEntry incl. nullable list forms.
        TeamEntry back;
        entryRoundTrip<TeamEntry>(
            [](TeamEntry& e) {
                e.v0_ = 1;
                e.b4_ = true;
                e.id16_ = LogicLong{5, 6};
                e.ref40_ = DataReference{16, 3};
                e.battleMap_ = std::make_unique<LogicBattlePlayerMap>();
                e.battleMap_->v16_ = 2;
                e.battleMap_->compressed_ = std::make_unique<LogicCompressedString>();
                auto m = std::make_unique<TeamMemberEntry>();
                m->display_ = std::make_unique<PlayerDisplayData>();
                e.members_.push_back(std::move(m));
                e.ints80_ = {1, 2};
                e.b72_ = true;
                e.intList_ = std::vector<i32>{9};
            },
            back);
        CHECK(back.v0_ == 1 && back.b4_ && !back.b5_ && !back.b6_);
        CHECK(back.id16_.low == 6 && back.ref40_.has_value());
        CHECK(back.battleMap_ && back.battleMap_->v16_ == 2);
        CHECK(!back.battleMap_->longs_.has_value());
        CHECK(back.members_.size() == 1 && back.invites_.empty());
        CHECK(back.joinRequests_.empty());
        CHECK(back.ints80_.size() == 2 && back.b72_ && !back.b73_ && !back.b74_);
        CHECK(back.intList_.has_value() && back.intList_->size() == 1);
    }
    {
        // Null battle map + null int list round-trip as absent.
        TeamEntry back;
        entryRoundTrip<TeamEntry>([](TeamEntry&) {}, back);
        CHECK(!back.battleMap_ && !back.intList_ && back.members_.empty());
    }
    // RankedMatch + misc wave (@0x25999c, @0x400e0c/@0x47c4c8, @0x5515b4, @0x27b558).
    {
        LogicRankedMatchPlayer back;
        entryRoundTrip<LogicRankedMatchPlayer>(
            [](LogicRankedMatchPlayer& e) {
                e.id8_ = LogicLong{1, 1};
                e.display_ = std::make_unique<PlayerDisplayData>();
                e.v24_ = 24;
                e.ref40_ = DataReference{16, 2};
                e.b80_ = true;
            },
            back);
        CHECK(back.id8_.low == 1 && back.display_ && back.v24_ == 24);
        CHECK(back.ref40_.has_value() && !back.ref56_ && back.b80_);
    }
    {
        LogicRankedMatch back;
        entryRoundTrip<LogicRankedMatch>(
            [](LogicRankedMatch& e) {
                e.uuid_.hi_ = 7;
                e.b40_ = true;
                e.players_.emplace();
                auto p = std::make_unique<LogicRankedMatchPlayer>();
                p->v32_ = 9;
                e.players_->push_back(std::move(p));
                e.v56_ = 56;
            },
            back);
        CHECK(back.uuid_.hi_ == 7 && !back.ref32_ && back.b40_);
        CHECK(back.players_.has_value() && back.players_->size() == 1);
        CHECK(back.players_->at(0)->v32_ == 9 && back.v56_ == 56);
    }
    {
        // Null players list round-trips as -1.
        LogicRankedMatch back;
        entryRoundTrip<LogicRankedMatch>([](LogicRankedMatch&) {}, back);
        CHECK(!back.players_.has_value());
    }
    {
        RewardEntry back;
        entryRoundTrip<RewardEntry>(
            [](RewardEntry& e) {
                e.v0_ = 1;
                e.b32_ = true;
                e.offer_ = std::make_unique<LogicGemOffer>();
                e.offer_->v0_ = 4;
            },
            back);
        CHECK(back.v0_ == 1 && back.b32_ && !back.offer2_);
        CHECK(back.offer_ && back.offer_->v0_ == 4);
    }
    {
        TeamInvitation back;
        entryRoundTrip<TeamInvitation>(
            [](TeamInvitation& e) {
                e.id0_ = LogicLong{9, 9};
                e.friend_ = std::make_unique<FriendEntry>();
                e.friend_->ints_[0] = 3;
            },
            back);
        CHECK(back.id0_.low == 9 && back.friend_ && back.friend_->ints_[0] == 3);
    }
    // RoundState + misc wave (@0x8e26bc, @0x8c31b0, @0x65bdf8, @0x639e40).
    {
        LogicRankedMatchRoundState back;
        entryRoundTrip<LogicRankedMatchRoundState>(
            [](LogicRankedMatchRoundState& e) {
                e.head_[0] = 1;
                e.head_[10] = 10;
                e.rewards1_.emplace();
                auto r = std::make_unique<LogicPlayerRewardData>();
                r->v16_ = 3;
                e.rewards1_->push_back(std::move(r));
                e.rankReward_ = std::make_unique<LogicRankRewardConfig>();
                e.rankReward_->v8_ = 8;
                e.debug_ = std::make_unique<LogicRankedMatchResultDebugInfo>();
                e.debug_->v_[5] = 5;
            },
            back);
        CHECK(back.head_[0] == 1 && back.head_[10] == 10);
        CHECK(back.rewards1_.has_value() && back.rewards1_->size() == 1);
        CHECK(!back.rewards2_.has_value());
        CHECK(back.rankReward_ && back.rankReward_->v8_ == 8);
        CHECK(back.debug_ && back.debug_->v_[5] == 5);
    }
    {
        TeamInvitationDataEntry back;
        entryRoundTrip<TeamInvitationDataEntry>(
            [](TeamInvitationDataEntry& e) {
                e.id0_ = LogicLong{4, 5};
                e.name_ = std::string("t");
            },
            back);
        CHECK(back.id0_.low == 5 && back.name_.value() == "t");
    }
    {
        ChronosFileEntry back;
        entryRoundTrip<ChronosFileEntry>(
            [](ChronosFileEntry& e) {
                e.s0_ = std::string("a.csv");
                e.s16_ = std::string("b.csv");
            },
            back);
        CHECK(back.s0_ == "a.csv" && back.s16_ == "b.csv");
    }
    // Log/TV/latency/lobby wave (@0x93cea4, @0x871e14, @0x726198, @0x263348).
    {
        BattleLogPlayerEntry back;
        entryRoundTrip<BattleLogPlayerEntry>(
            [](BattleLogPlayerEntry& e) {
                e.v0_ = 1;
                e.id8_ = LogicLong{2, 3};
                e.b20_ = true;
                e.v40_ = 40;
                e.display_ = std::make_unique<PlayerDisplayData>();
            },
            back);
        CHECK(back.v0_ == 1 && back.id8_.low == 3 && back.b20_);
        CHECK(!back.ref24_ && back.v40_ == 40 && back.display_);
    }
    {
        BrawlTvChannelInfo back;
        entryRoundTrip<BrawlTvChannelInfo>(
            [](BrawlTvChannelInfo& e) {
                e.v0_ = 7;
                e.s8_ = std::string("ch");
            },
            back);
        CHECK(back.v0_ == 7 && back.s8_.value() == "ch" && !back.s16_);
    }
    {
        LatencyData back;
        entryRoundTrip<LatencyData>(
            [](LatencyData& e) {
                e.v0_ = 1;
                e.v12_ = 12;
                e.b16_ = true;
                e.ts_ = 0x0123456789ABCDEFULL;
                e.s48_ = std::string("eu");
            },
            back);
        CHECK(back.v0_ == 1 && back.v12_ == 12 && back.b16_);
        CHECK(back.ts_ == 0x0123456789ABCDEFULL);
        CHECK(!back.s32_ && !back.s40_ && back.s48_.value() == "eu");
    }
    {
        LobbyInfoEntry back;
        entryRoundTrip<LobbyInfoEntry>(
            [](LobbyInfoEntry& e) { e.v_[4] = 42; }, back);
        CHECK(back.v_[0] == 0 && back.v_[4] == 42);
    }
    // Status/latency wave (@0x1a39e8/@0x26a79c, @0x3ffdc4, @0x94bcfc, @0x3af3e4).
    {
        FriendOnlineStatusEntry back;
        entryRoundTrip<FriendOnlineStatusEntry>(
            [](FriendOnlineStatusEntry& e) {
                e.id0_ = LogicLong{1, 2};
                e.v16_ = 16;
                e.b24_ = true;
                e.team_ = std::make_unique<AllianceTeamEntry>();
                e.team_->v0_ = 3;
            },
            back);
        CHECK(back.id0_.low == 2 && back.v16_ == 16 && back.b24_);
        CHECK(back.team_ && back.team_->v0_ == 3);
    }
    {
        // The old stub name is now an alias of the real class.
        FriendOnlineStatus aliasBack;
        entryRoundTrip<FriendOnlineStatus>(
            [](FriendOnlineStatus& e) { e.v20_ = 5; }, aliasBack);
        CHECK(aliasBack.v20_ == 5 && !aliasBack.team_);
    }
    {
        LatencyTestConfiguration back;
        entryRoundTrip<LatencyTestConfiguration>(
            [](LatencyTestConfiguration& e) {
                e.head_[0] = 1;
                e.head_[6] = 6;
                e.b28_ = true;
                e.b48_ = true;
                e.v32_ = 32;
                e.s40_ = std::string("cfg");
                e.bytes64_ = {0x01, 0x02};
                e.v52_ = 52;
            },
            back);
        CHECK(back.head_[0] == 1 && back.head_[6] == 6);
        CHECK(back.b28_ && !back.b29_ && back.b48_ && back.v32_ == 32);
        CHECK(back.s40_.value() == "cfg" && back.bytes64_.size() == 2);
        CHECK(back.bytes80_.empty() && back.v52_ == 52 && !back.s56_);
    }
    {
        PlayAgainStatus back;
        entryRoundTrip<PlayAgainStatus>(
            [](PlayAgainStatus& e) {
                e.ids_.push_back(LogicLong{0, 11});
                e.ids_.push_back(LogicLong{0, 22});
            },
            back);
        CHECK(back.ids_.size() == 2 && back.ids_[1].low == 22);
    }
    {
        StatusChangeEntry back;
        entryRoundTrip<StatusChangeEntry>(
            [](StatusChangeEntry& e) {
                e.id8_ = LogicLong{8, 8};
                e.v0_ = 1;
            },
            back);
        CHECK(back.id8_.low == 8 && back.v0_ == 1);
    }
    // Tencent instruction (@0x8f84e0, embedded stringrefs).
    {
        LogicTencentAntiAddictionInstruction back;
        entryRoundTrip<LogicTencentAntiAddictionInstruction>(
            [](LogicTencentAntiAddictionInstruction& e) {
                e.v8_ = 1;
                e.s16_ = std::string("a");
                e.s32_ = std::string("b");
                e.s48_ = std::string("c");
                e.v64_ = 64;
                e.s72_ = std::string("d");
            },
            back);
        CHECK(back.v8_ == 1 && back.s16_ == "a" && back.s48_ == "c");
        CHECK(back.v64_ == 64 && back.s72_ == "d");
    }
    // Notifications (@0x7beae0/@0x605478, FreeText @0x2b1cf4/@0x8b24a8, type 81).
    {
        CHECK(createNotificationByType(81) != nullptr);
        CHECK(createNotificationByType(999) == nullptr);
        CHECK(createNotificationByType(81)->notificationType() == 81);
        // Full factory map: every mapped id builds and self-reports.
        for (int id : {63, 70, 64, 65, 66, 67, 68, 69, 71, 72, 73, 74, 75,
                       76, 77, 78, 79, 80, 81, 82, 84, 85, 86, 88, 89, 90,
                       91, 92, 93, 94}) {
            auto n = createNotificationByType(id);
            CHECK(n != nullptr);
            if (n && id != 63) CHECK(n->notificationType() == id);
        }
        // Unmapped ids (2/Donate has no factory case; 83/87 open) -> null.
        CHECK(createNotificationByType(2) == nullptr);
        CHECK(createNotificationByType(83) == nullptr);
        CHECK(createNotificationByType(87) == nullptr);
        FreeTextNotification back;
        entryRoundTrip<FreeTextNotification>(
            [](FreeTextNotification& e) {
                e.v8_ = 1;
                e.b12_ = true;
                e.v16_ = 2;
                e.s24_ = std::string("hi");
                e.v48_ = 48;
            },
            back);
        CHECK(back.v8_ == 1 && back.b12_ && back.v16_ == 2);
        CHECK(back.s24_.value() == "hi" && back.v48_ == 48);
    }
    {
        // Representative shape round-trips (one per wire shape).
        {
            GemRewardNotification back;
            entryRoundTrip<GemRewardNotification>(
                [](GemRewardNotification& e) {
                    e.v8_ = 1;
                    e.s24_ = std::string("g");
                    e.v48_ = 10;
                    e.v52_ = 20;
                },
                back);
            CHECK(back.v48_ == 10 && back.v52_ == 20 && back.notificationType() == 89);
        }
        {
            BoxRewardNotification back;
            entryRoundTrip<BoxRewardNotification>(
                [](BoxRewardNotification& e) { e.v56_ = 7; }, back);
            CHECK(back.v56_ == 7 && back.notificationType() == 64);
        }
        {
            DonateNotification back;
            entryRoundTrip<DonateNotification>(
                [](DonateNotification& e) { e.s48_ = std::string("ty"); }, back);
            CHECK(back.s48_.value() == "ty" && back.notificationType() == 2);
        }
        {
            BandNotification back;
            entryRoundTrip<BandNotification>(
                [](BandNotification& e) {
                    e.display_ = std::make_unique<PlayerDisplayData>();
                },
                back);
            CHECK(back.display_ && back.notificationType() == 82);
        }
        {
            RankedSeasonEndNotification back;
            entryRoundTrip<RankedSeasonEndNotification>(
                [](RankedSeasonEndNotification& e) {
                    e.v56_ = 1;
                    e.offer_ = std::make_unique<LogicGemOffer>();
                    e.offer_->v4_ = 4;
                },
                back);
            CHECK(back.v56_ == 1 && back.offer_ && back.offer_->v4_ == 4);
        }
        {
            StarPointsNotification back;
            entryRoundTrip<StarPointsNotification>(
                [](StarPointsNotification& e) {
                    auto s = std::make_unique<ScoreEntry>();
                    s->v_[3] = 30;
                    e.scores_.push_back(std::move(s));
                },
                back);
            CHECK(back.scores_.size() == 1 && back.scores_[0]->v_[3] == 30);
        }
        {
            RevokeNotification back;
            entryRoundTrip<RevokeNotification>(
                [](RevokeNotification& e) {
                    e.ts_ = 123456789ULL;
                    e.s72_ = std::string("r");
                },
                back);
            CHECK(back.ts_ == 123456789ULL && back.s72_.value() == "r");
        }
        {
            SkinPurchaseOptionRewardNotification back;
            entryRoundTrip<SkinPurchaseOptionRewardNotification>(
                [](SkinPurchaseOptionRewardNotification& e) {
                    e.ref56_ = DataReference{16, 1};
                    e.v64_ = 9;
                },
                back);
            CHECK(back.ref56_.has_value() && back.v64_ == 9);
        }
        {
            RankedMidSeasonRewardNotification back;
            entryRoundTrip<RankedMidSeasonRewardNotification>(
                [](RankedMidSeasonRewardNotification& e) {
                    e.config_ = std::make_unique<LogicRewardConfig>();
                },
                back);
            CHECK(back.config_ && back.notificationType() == 67);
        }
    }
    {
        // ClientHome with a live notification (closes the home loop).
        LogicClientHome back;
        entryRoundTrip<LogicClientHome>(
            [](LogicClientHome& h) {
                h.daily_ = std::make_unique<LogicDailyData>();
                h.daily_->forced_ = std::make_unique<ForcedDrops>();
                h.conf_ = std::make_unique<LogicConfData>();
                auto n = createNotificationByType(81);
                n->v8_ = 5;
                h.notifications_.emplace_back(81, std::move(n));
            },
            back);
        CHECK(back.daily_ && back.conf_);
        CHECK(back.notifications_.size() == 1);
        CHECK(back.notifications_[0].first == 81);
        CHECK(back.notifications_[0].second->v8_ == 5);
    }
    {
        // Unknown notification types still throw loudly.
        LogicClientHome back;
        LogicClientHome h;
        h.daily_ = std::make_unique<LogicDailyData>();
        h.daily_->forced_ = std::make_unique<ForcedDrops>();
        h.conf_ = std::make_unique<LogicConfData>();
        h.notifications_.emplace_back(999, std::make_unique<BaseNotification>());
        bool threw = false;
        try {
            ByteStream enc;
            h.encode(enc);
            ByteStream d;
            d.setBuffer(enc.data(), enc.size());
            back.decode(d);
        } catch (const pending_reverse&) {
            threw = true;
        }
        CHECK(threw);
    }
    // Ranking wave (@0x8a6d9c, @0x6ef580, @0x770f68/@0x88e8d8).
    {
        PlayerRankingData back;
        entryRoundTrip<PlayerRankingData>(
            [](PlayerRankingData& e) {
                e.name_ = std::string("p");
                e.display_ = std::make_unique<PlayerDisplayData>();
            },
            back);
        CHECK(back.name_.value() == "p" && back.display_);
    }
    {
        AllianceRankingData back;
        entryRoundTrip<AllianceRankingData>(
            [](AllianceRankingData& e) {
                e.name_ = std::string("a");
                e.v16_ = 16;
                e.ref24_ = DataReference{16, 2};
            },
            back);
        CHECK(back.name_.value() == "a" && back.v16_ == 16);
        CHECK(back.ref24_.has_value());
    }
    {
        RankingEntry back;
        entryRoundTrip<RankingEntry>(
            [](RankingEntry& e) {
                e.id8_ = LogicLong{0, 99};
                e.v16_ = 1;
                e.v20_ = 2;
                e.player_ = std::make_unique<PlayerRankingData>();
                e.player_->display_ = std::make_unique<PlayerDisplayData>();
            },
            back);
        CHECK(back.id8_.low == 99 && back.v16_ == 1 && back.v20_ == 2);
        CHECK(back.player_ && !back.alliance_);
    }
    // LogicRewards (@0x2c2fac) closes two notification loops.
    {
        LogicRewards back;
        entryRoundTrip<LogicRewards>(
            [](LogicRewards& e) {
                auto o = std::make_unique<LogicGemOffer>();
                o->v0_ = 2;
                e.offers_.push_back(std::move(o));
            },
            back);
        CHECK(back.offers_.size() == 1 && back.offers_[0]->v0_ == 2);
    }
    {
        ChallengeRewardNotification back;
        entryRoundTrip<ChallengeRewardNotification>(
            [](ChallengeRewardNotification& e) {
                e.rewards_ = std::make_unique<LogicRewards>();
                e.v48_ = 1;
                e.v64_ = 1;
                e.s72_ = std::string("ch");
            },
            back);
        CHECK(back.rewards_ && back.v48_ == 1 && back.s72_.value() == "ch");
        CHECK(back.notificationType() == 63);
    }
    {
        BrawlPassAutoCollectSeasonNotification back;
        entryRoundTrip<BrawlPassAutoCollectSeasonNotification>(
            [](BrawlPassAutoCollectSeasonNotification& e) {
                e.rewards_ = std::make_unique<LogicRewards>();
                e.v48_ = 9;
            },
            back);
        CHECK(back.rewards_ && back.v48_ == 9 && back.notificationType() == 69);
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
