#pragma once

// Command batch B (types 500-542) — reversed from libg_decrypted.so.
// All bodies are header-inline (scalar shapes).

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <optional>
#include <string>

namespace titan {

#define TITAN_CMD_VINT(Name, Id, Field)                                         \
    class Name : public LogicCommand {                                          \
    public:                                                                    \
        int getCommandType() const override { return Id; }                     \
        void encode(ByteStream& s) const override {                            \
            LogicCommand::encode(s);                                           \
            s.writeVInt(Field);                                                \
        }                                                                      \
        void decode(ByteStream& s) override {                                  \
            LogicCommand::decode(s);                                           \
            Field = s.readVInt();                                              \
        }                                                                      \
        i32 v_ = 0;                                                            \
    }

#define TITAN_CMD_EMPTY(Name, Id)                                               \
    class Name : public LogicCommand {                                          \
    public:                                                                    \
        int getCommandType() const override { return Id; }                     \
    }

#define TITAN_CMD_DATAREF(Name, Id, Field)                                      \
    class Name : public LogicCommand {                                          \
    public:                                                                    \
        int getCommandType() const override { return Id; }                     \
        void encode(ByteStream& s) const override {                            \
            LogicCommand::encode(s);                                           \
            DataReference::encodeNullable(s, Field);                           \
        }                                                                      \
        void decode(ByteStream& s) override {                                  \
            LogicCommand::decode(s);                                           \
            Field = DataReference::decodeNullable(s);                          \
        }                                                                      \
        std::optional<DataReference> Field;                                    \
    }

// ---- 500 LogicGatchaCommand: vint ----
TITAN_CMD_VINT(LogicGatchaCommand, 500, v_);

// ---- 503 LogicClaimDailyRewardCommand: vint + vint ----
class LogicClaimDailyRewardCommand : public LogicCommand {
public:
    int getCommandType() const override { return 503; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 505 LogicSetPlayerThumbnailCommand: dataref ----
TITAN_CMD_DATAREF(LogicSetPlayerThumbnailCommand, 505, ref_);

// ---- 506 LogicSelectSkinCommand: dataref ----
TITAN_CMD_DATAREF(LogicSelectSkinCommand, 506, ref_);

// ---- 507 LogicUnlockSkinCommand: dataref ----
TITAN_CMD_DATAREF(LogicUnlockSkinCommand, 507, ref_);

// ---- 508 LogicChangeControlModeCommand: vint ----
TITAN_CMD_VINT(LogicChangeControlModeCommand, 508, v_);

// ---- 509 LogicPurchaseDoubleCoinsCommand: empty ----
TITAN_CMD_EMPTY(LogicPurchaseDoubleCoinsCommand, 509);

// ---- 512 LogicToggleInGameHintsCommand: empty ----
TITAN_CMD_EMPTY(LogicToggleInGameHintsCommand, 512);

// ---- 514 LogicDeleteNotificationCommand: vint + vint ----
class LogicDeleteNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 514; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 515 LogicClearShopTickersCommand: vint x4 ----
class LogicClearShopTickersCommand : public LogicCommand {
public:
    int getCommandType() const override { return 515; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        for (i32 x : v_) s.writeVInt(x);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        for (i32& x : v_) x = s.readVInt();
    }
    i32 v_[4] = {};
};

// ---- 517 LogicClaimRankUpRewardCommand: vint + dataref + vint + vint ----
class LogicClaimRankUpRewardCommand : public LogicCommand {
public:
    int getCommandType() const override { return 517; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v2_);
        s.writeVInt(v3_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
        v2_ = s.readVInt();
        v3_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
    std::optional<DataReference> ref_;
};

// ---- 519 LogicPurchaseOfferCommand: vint + dataref ----
class LogicPurchaseOfferCommand : public LogicCommand {
public:
    int getCommandType() const override { return 519; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        DataReference::encodeNullable(s, ref_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
    }
    i32 v_ = 0;
    std::optional<DataReference> ref_;
};

// ---- 520 LogicLevelUpCommand: dataref ----
TITAN_CMD_DATAREF(LogicLevelUpCommand, 520, ref_);

// ---- 521 LogicPurchaseHeroLvlUpMaterialCommand: vint ----
TITAN_CMD_VINT(LogicPurchaseHeroLvlUpMaterialCommand, 521, v_);

// ---- 522 LogicHeroSeenCommand: dataref + int ----
class LogicHeroSeenCommand : public LogicCommand {
public:
    int getCommandType() const override { return 522; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        DataReference::encodeNullable(s, ref_);
        s.writeInt(i_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        ref_ = DataReference::decodeNullable(s);
        i_ = s.readInt();
    }
    std::optional<DataReference> ref_;
    i32 i_ = 0;
};

// ---- 523 LogicClaimAdRewardCommand: vint + vint ----
class LogicClaimAdRewardCommand : public LogicCommand {
public:
    int getCommandType() const override { return 523; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 524 LogicVideoStartedCommand: vint ----
TITAN_CMD_VINT(LogicVideoStartedCommand, 524, v_);

// ---- 525 LogicSelectCharacterCommand: dataref ----
TITAN_CMD_DATAREF(LogicSelectCharacterCommand, 525, ref_);

// ---- 526 LogicUnlockFreeSkinsCommand: empty ----
TITAN_CMD_EMPTY(LogicUnlockFreeSkinsCommand, 526);

// ---- 527 LogicSetPlayerNameColorCommand: dataref ----
TITAN_CMD_DATAREF(LogicSetPlayerNameColorCommand, 527, ref_);

// ---- 528 LogicViewInboxNotificationCommand: vint + vint ----
class LogicViewInboxNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 528; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 529 LogicSelectStarPowerCommand: dataref ----
TITAN_CMD_DATAREF(LogicSelectStarPowerCommand, 529, ref_);

// ---- 530 LogicSetPlayerAgeCommand: int ----
class LogicSetPlayerAgeCommand : public LogicCommand {
public:
    int getCommandType() const override { return 530; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readInt();
    }
    i32 v_ = 0;
};

// ---- 531 LogicCancelPurchaseOfferCommand: vint + vint ----
class LogicCancelPurchaseOfferCommand : public LogicCommand {
public:
    int getCommandType() const override { return 531; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 532 LogicItemSeenCommand: dataref ----
TITAN_CMD_DATAREF(LogicItemSeenCommand, 532, ref_);

// ---- 533 LogicQuestsSeenCommand: empty ----
TITAN_CMD_EMPTY(LogicQuestsSeenCommand, 533);

// ---- 534 LogicPurchaseBrawlPassCommand: vint + bool ----
class LogicPurchaseBrawlPassCommand : public LogicCommand {
public:
    int getCommandType() const override { return 534; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        s.writeBoolean(b_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        b_ = s.readBoolean();
    }
    i32 v_ = 0;
    bool b_ = false;
};

// ---- 535 LogicClaimTailRewardCommand: vint + vint ----
class LogicClaimTailRewardCommand : public LogicCommand {
public:
    int getCommandType() const override { return 535; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 536 LogicPurchaseBrawlPassProgressCommand: vint ----
TITAN_CMD_VINT(LogicPurchaseBrawlPassProgressCommand, 536, v_);

// ---- 537 LogicVanityItemSeenCommand: dataref ----
TITAN_CMD_DATAREF(LogicVanityItemSeenCommand, 537, ref_);

// ---- 538 LogicSelectEmoteCommand: dataref + vint ----
class LogicSelectEmoteCommand : public LogicCommand {
public:
    int getCommandType() const override { return 538; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        ref_ = DataReference::decodeNullable(s);
        v_ = s.readVInt();
    }
    std::optional<DataReference> ref_;
    i32 v_ = 0;
};

// ---- 539 LogicBrawlPassAutoCollectWarningSeenCommand: vint ----
TITAN_CMD_VINT(LogicBrawlPassAutoCollectWarningSeenCommand, 539, v_);

// ---- 540 LogicPurchaseChallengeLivesCommand: empty ----
TITAN_CMD_EMPTY(LogicPurchaseChallengeLivesCommand, 540);

// ---- 541 LogicClearESportsHubNotificationCommand: empty ----
TITAN_CMD_EMPTY(LogicClearESportsHubNotificationCommand, 541);

// ---- 542 LogicSelectGroupSkinCommand: dataref ----
TITAN_CMD_DATAREF(LogicSelectGroupSkinCommand, 542, ref_);

#undef TITAN_CMD_VINT
#undef TITAN_CMD_EMPTY
#undef TITAN_CMD_DATAREF

} // namespace titan
