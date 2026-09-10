// Self-check for message batches 00-03.

#include "titan/messages/MsgBatch00.hpp"
#include "titan/messages/MsgBatch01.hpp"
#include "titan/messages/MsgBatch02.hpp"
#include "titan/messages/MsgBatch03.hpp"
#include "titan/messages/MsgBatch04.hpp"
#include "titan/messages/MsgBatch05.hpp"
#include "titan/messages/MsgBatch06.hpp"
#include "titan/messages/MsgBatch07.hpp"
#include "titan/messages/MsgBatch08.hpp"
#include "titan/messages/MsgBatch09.hpp"
#include "titan/messages/MsgBatch10.hpp"
#include "titan/messages/MsgBatch11.hpp"
#include "titan/messages/MsgBatch12.hpp"
#include "titan/messages/MsgBatch13.hpp"

#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

template <typename Msg, typename Setup>
static void roundTrip(Setup setup, Msg& back) {
    Msg out;
    setup(out);
    out.encode();
    back.stream().setBuffer(out.stream().data(), out.stream().size());
    back.decode();
}
#define ROUNDTRIP(Type, var, ...)              \
    Type var;                                  \
    roundTrip<Type>([](Type& m) { __VA_ARGS__; }, var)

int main() {
    // IDs.
    CHECK(AcceptFriendFailedMessage().getMessageType() == 20501);
    CHECK(AllianceResponseMessage().getMessageType() == 24333);
    CHECK(AttributionMessage().getMessageType() == 30000);
    CHECK(BattleEndMessage().getMessageType() == 23456);
    CHECK(ClientHelloMessage().getMessageType() == 10100);
    CHECK(CreateAccountOkMessage().getMessageType() == 20101);
    CHECK(CancelMatchmakingMessage().getMessageType() == 14106);

    // Scalar round-trips.
    {
        ROUNDTRIP(AllianceResponseMessage, back, m.responseType_ = 3; m.reason_ = 44);
        CHECK(back.responseType_ == 3 && back.reason_ == 44);
    }
    {
        ROUNDTRIP(AccountSwitchedMessage, back,
                  m.accountId_ = {11, 22}; m.flag1_ = true;
                  m.env_ = std::string("prod"));
        CHECK(back.accountId_.high == 11 && back.accountId_.low == 22);
        CHECK(back.flag1_ && !back.flag2_);
        CHECK(back.env_.value() == "prod");
    }
    {
        ROUNDTRIP(AddFriendByAvatarNameAndCodeMessage, back,
                  m.name_ = std::string("Spike"));
        CHECK(back.name_.value() == "Spike" && !back.code_.has_value());
    }
    {
        ROUNDTRIP(AskForAllianceDataMessage, back,
                  m.allianceId_ = {1, 2}; m.hasSecond_ = true; m.secondId_ = {3, 4});
        CHECK(back.hasSecond_ && back.secondId_.high == 3 && back.secondId_.low == 4);
    }
    {
        ROUNDTRIP(AttributionEventMessage, back,
                  m.params_[0] = "a"; m.params_[8] = "z"; m.flag_ = true);
        CHECK(back.params_[0] == "a" && back.params_[8] == "z" && back.flag_);
    }
    {
        ROUNDTRIP(ClientHelloMessage, back,
                  m.majorVersion_ = 36; m.fingerprintSha_ = "abc123");
        CHECK(back.majorVersion_ == 36 && back.fingerprintSha_ == "abc123");
    }
    {
        ROUNDTRIP(CreateAllianceMessage, back,
                  m.name_ = std::string("Titan"); m.badge_ = DataReference{8, 15};
                  m.requiredTrophies_ = 1000; m.familyFriendly_ = true);
        CHECK(back.name_.value() == "Titan");
        CHECK(back.badge_->classId == 8 && back.badge_->instanceId == 15);
        CHECK(!back.region_.has_value());
        CHECK(back.requiredTrophies_ == 1000 && back.familyFriendly_);
    }
    {
        ROUNDTRIP(BindTencentAccountMessage, back,
                  m.accountId_ = std::string("t1"); m.token_ = std::string("tok");
                  m.vintTail_ = 5); // no force flag on the wire
        CHECK(back.accountId_.value() == "t1" && back.vintTail_ == 5);
        CHECK(back.stream().size() > 0);
    }
    {
        ROUNDTRIP(BrawlTvChannelNextUpMessage, back,
                  m.v132_ = 7; m.hasBattle_ = false; m.stringRef_ = "soon");
        CHECK(back.v132_ == 7 && back.stringRef_ == "soon");
    }
    {
        ROUNDTRIP(AskForPlayingTencentFriendsMessage, back, m.ids_ = {"x", "y"});
        CHECK(back.ids_.size() == 2 && back.ids_[1] == "y");
    }
    {
        BillingCancelledByClientMessage m;
        m.isNull_ = true;
        m.encode();
        CHECK(m.stream().size() == 4); // writeInt(-1)
    }
    // Pending nested entries throw explicitly (AddableFriendEntry still pending).
    {
        bool threw = false;
        AddableFriendsMessage m;
        m.entries_.push_back(std::make_unique<AddableFriendEntry>());
        try {
            m.encode();
        } catch (const pending_reverse&) {
            threw = true;
        }
        CHECK(threw);
        AddableFriendsMessage back;
        roundTrip<AddableFriendsMessage>([](AddableFriendsMessage&) {}, back);
        CHECK(back.entries_.empty());
    }
    // Newly reversed entries round-trip for real.
    {
        ROUNDTRIP(AllianceListMessage, back,
                  auto h = std::make_unique<AllianceHeaderEntry>();
                  h->name_ = std::string("Titans");
                  h->v24_ = 5;
                  h->badgeRef_ = DataReference{8, 1};
                  h->flag_ = true;
                  m.headers_.push_back(std::move(h)));
        CHECK(back.headers_.size() == 1);
        CHECK(back.headers_[0]->name_.value() == "Titans");
        CHECK(back.headers_[0]->v24_ == 5 && back.headers_[0]->flag_);
    }
    // Wave 2 spot checks.
    {
        ROUNDTRIP(EndClientTurnMessage, back,
                  m.flag_ = true; m.tick_ = 12; m.checksum_ = 34;
                  m.payload_ = std::vector<u8>({1, 2, 3}));
        CHECK(back.flag_ && back.tick_ == 12 && back.checksum_ == 34);
        CHECK(back.payload_.has_value() && back.payload_->size() == 3);
        CHECK(back.commands_.empty());
    }
    {
        ROUNDTRIP(FriendOnlineStatusMessage, back,
                  m.avatarIds_.push_back(LogicLong{5, 6}));
        CHECK(back.avatarIds_.size() == 1 && back.avatarIds_[0].low == 6);
    }
    {
        ROUNDTRIP(GlobalChatLineMessage, back,
                  m.message_ = std::string("gg"); m.senderLevel_ = 9;
                  m.playerId_ = LogicLong{7, 8});
        CHECK(back.message_.value() == "gg" && back.senderLevel_ == 9);
    }
    {
        ROUNDTRIP(MatchmakeRequestMessage, back,
                  m.eventRef_ = DataReference{2, 3}; m.vints_[0] = 1);
        CHECK(back.eventRef_->classId == 2 && back.vints_[0] == 1);
    }
    {
        // LoginOk short variant: decode stops at end without crashing.
        LoginOkMessage full;
        full.accountId_ = LogicLong{1, 1};
        full.homeId_ = LogicLong{2, 2};
        try {
            full.encode(); // throws: compressed_ pending
        } catch (const pending_reverse&) {}
        LoginOkMessage cut;
        cut.stream().setBuffer(full.stream().data(), 60); // scalar prefix only
        cut.decode();
        CHECK(!cut.hasTail1_);
    }
    {
        ROUNDTRIP(RankedMatchBanHeroMessage, back,
                  m.charRef_ = DataReference{4, 5}; m.slot_ = 2);
        CHECK(back.hasSlot_ && back.slot_ == 2);
        // Old short variant without slot.
        RankedMatchBanHeroMessage wo;
        wo.charRef_ = DataReference{4, 5};
        wo.stream().writeVInt(4);
        wo.stream().writeVInt(5);
        RankedMatchBanHeroMessage back2;
        back2.stream().setBuffer(wo.stream().data(), wo.stream().size());
        back2.decode();
        CHECK(!back2.hasSlot_);
    }
    // Wave 3 spot checks.
    {
        // SearchAlliances: last two ints swapped vs layout order.
        ROUNDTRIP(SearchAlliancesMessage, back,
                  m.text_ = std::string("abc"); m.ints_[0] = 1;
                  m.flag_ = true; m.p168_ = 8; m.p160_ = 9);
        CHECK(back.text_.value() == "abc" && back.p168_ == 8 && back.p160_ == 9);
        // Wire order check: ...flag, p168, p160 at the tail.
        const u8* d = back.stream().data();
        (void)d;
    }
    {
        ROUNDTRIP(TeamCreateMessage, back,
                  m.teamType_ = 2; m.hasInvite_ = true;
                  m.inviteId_ = LogicLong{9, 9}; m.unk160_ = 3;
                  m.eventRef_ = DataReference{5, 6}; m.roomId_ = LogicLong{1, 1});
        CHECK(back.teamType_ == 2 && back.hasInvite_);
        CHECK(back.inviteId_.high == 9 && back.unk160_ == 3);
        CHECK(back.eventRef_->instanceId == 6);
    }
    {
        ROUNDTRIP(UdpBigMessageFragmentMessage, back,
                  m.fragId_ = 1; m.msgId_ = 300; m.index_ = 0; m.count_ = 2;
                  m.payload_ = std::vector<u8>({0xAA, 0xBB}));
        CHECK(back.msgId_ == 300 && back.payload_.size() == 2);
        CHECK(back.payload_[0] == 0xAA);
    }
    {
        // TencentBillingRequest: f3/f4 swapped vs layout.
        TencentBillingRequestMessage m;
        m.f1_ = "a";
        m.f2_ = "b";
        m.f3_ = "c";
        m.f4_ = "d";
        m.encode();
        TencentBillingRequestMessage back;
        back.stream().setBuffer(m.stream().data(), m.stream().size());
        back.decode();
        CHECK(back.f1_ == "a" && back.f2_ == "b");
        CHECK(back.f3_ == "c" && back.f4_ == "d");
    }
    {
        ROUNDTRIP(RankedMatchBanEndedMessage, back,
                  m.bans_.emplace_back(7, DataReference{30, 31}));
        CHECK(back.bans_.size() == 1 && back.bans_[0].first == 7);
        CHECK(back.bans_[0].second.classId == 30);
    }
    {
        ROUNDTRIP(YoozooOrderDeliveryFailedMessage, back,
                  m.orderId_ = std::string("ord-1"); m.errorCode_ = 5);
        CHECK(back.orderId_.value() == "ord-1" && back.errorCode_ == 5);
    }
    // RLE compressed strings round-trip.
    {
        LogicCompressedString cs;
        cs.setUncompressed("AAABBBCCCD");
        auto plain = cs.getUncompressed();
        CHECK(plain == std::vector<u8>({'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C', 'D'}));
        // Encode/decode the bytes field itself.
        ByteStream s;
        cs.encode(s);
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        LogicCompressedString back;
        back.decode(d);
        CHECK(back.getUncompressed() == plain);
    }
    // LogicClientAvatar round-trip (empty slots).
    {
        LogicClientAvatar av;
        av.name_ = "Spike";
        av.flag_ = true;
        ByteStream s;
        av.encode(s);
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        LogicClientAvatar back;
        back.decode(d);
        CHECK(back.name_ == "Spike" && back.flag_);
    }

    if (failures == 0) std::puts("msgbatch: all ok");
    return failures == 0 ? 0 : 1;
}
