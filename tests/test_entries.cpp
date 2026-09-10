// Self-check for reversed nested entries and stream factories.

#include "titan/game/AddableFriendEntry.hpp"
#include "titan/game/AllianceEventStreamEntry.hpp"
#include "titan/game/AllianceTeamEntry.hpp"
#include "titan/game/AvatarStreamEntry.hpp"
#include "titan/game/BattleLogEntry.hpp"
#include "titan/game/ChatStreamEntry.hpp"
#include "titan/game/EventData.hpp"
#include "titan/game/FriendEntry.hpp"
#include "titan/game/JoinRequestAllianceStreamEntry.hpp"
#include "titan/game/LogicClientHome.hpp"
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

    if (failures == 0) std::puts("entries: all ok");
    return failures == 0 ? 0 : 1;
}
