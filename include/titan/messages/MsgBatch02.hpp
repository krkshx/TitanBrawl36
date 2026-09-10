#pragma once

// Message batch 02 — reversed from libg_decrypted.so (ARM64).
// Classes: AvatarNameCheckRequest .. CancelMatchmaking.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(AvatarStreamEntry);
TITAN_PENDING_ENTRY(PlayerEntry);
TITAN_PENDING_ENTRY(XpEntry);
TITAN_PENDING_ENTRY(LogicMilestoneProgress);
TITAN_PENDING_ENTRY(LogicGemOffer);
TITAN_PENDING_ENTRY(PlayAgainStatus);
TITAN_PENDING_ENTRY(LogicQuests);
TITAN_PENDING_ENTRY(LogicRankedMatchRoundState);
TITAN_PENDING_ENTRY(ChronosTextEntry);
TITAN_PENDING_ENTRY(BattleLogEntry);
TITAN_PENDING_ENTRY(BattleReportStreamEntry);
TITAN_PENDING_ENTRY(BrawlTvChannelInfo);

// ---- 14600 AvatarNameCheckRequestMessage: string ----
class AvatarNameCheckRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14600; }
    const char* getMessageTypeName() const override { return "AvatarNameCheckRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        name_ = stream().readString();
    }
    std::optional<std::string> name_;
};

// ---- 20300 AvatarNameCheckResponseMessage ----
class AvatarNameCheckResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20300; }
    const char* getMessageTypeName() const override { return "AvatarNameCheckResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(success_);
        stream().writeInt(errorCode_);
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        success_ = stream().readBoolean();
        errorCode_ = stream().readInt();
        name_ = stream().readString();
    }
    bool success_ = false;
    i32 errorCode_ = 0;
    std::optional<std::string> name_;
};

// ---- 24412 AvatarStreamEntryMessage ----
class AvatarStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24412; }
    const char* getMessageTypeName() const override { return "AvatarStreamEntryMessage"; }
    void encode() override;
    void decode() override;
    i32 entryType_ = 0;
    std::unique_ptr<AvatarStreamEntry> entry_;
};

// ---- 24418 AvatarStreamEntryRemovedMessage: logiclong ----
class AvatarStreamEntryRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24418; }
    const char* getMessageTypeName() const override { return "AvatarStreamEntryRemovedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        entryId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        entryId_ = LogicLong::decode(stream());
    }
    LogicLong entryId_;
};

// ---- 24411 AvatarStreamMessage: count(-1=null) + typed entries ----
class AvatarStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24411; }
    const char* getMessageTypeName() const override { return "AvatarStreamMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::pair<i32, std::unique_ptr<AvatarStreamEntry>>> entries_;
    bool isNull_ = false;
};

// ---- 23456 BattleEndMessage ----
class BattleEndMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23456; }
    const char* getMessageTypeName() const override { return "BattleEndMessage"; }
    void encode() override;
    void decode() override;
    i64 battleIdA_ = 0, battleIdB_ = 0;
    i32 v11_[11] = {}; // v152..v192 in wire order
    bool hasGemOffer_ = false;
    std::unique_ptr<LogicGemOffer> gemOffer_;
    i32 v196_ = 0, v200_ = 0;
    bool b204_ = false;
    i32 v216_ = 0, v220_ = 0;
    bool flags_[6] = {}; // b224..b229
    i32 v232_ = 0;
    bool b236_ = false;
    std::vector<std::unique_ptr<PlayerEntry>> players_;
    std::vector<std::unique_ptr<XpEntry>> xp_;
    std::vector<DataReference> dataRefs_;
    std::vector<std::unique_ptr<LogicMilestoneProgress>> milestones_;
    std::optional<DataReference> dataRef272_;
    bool hasPlayAgain_ = false;
    std::unique_ptr<PlayAgainStatus> playAgain_;
    bool hasQuests_ = false;
    std::unique_ptr<LogicQuests> quests_;
    i32 v304_ = 0, v308_ = 0;
    bool hasRankedState_ = false;
    std::unique_ptr<LogicRankedMatchRoundState> rankedState_;
    i32 v312_ = 0;
    bool hasChronos_ = false;
    std::unique_ptr<ChronosTextEntry> chronos_;
};

// ---- 23458 BattleLogMessage: bool + entries ----
class BattleLogMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23458; }
    const char* getMessageTypeName() const override { return "BattleLogMessage"; }
    void encode() override;
    void decode() override;
    bool b132_ = false;
    std::vector<std::unique_ptr<BattleLogEntry>> entries_;
};

// ---- 23459 BattleLogReplayAvailableMessage ----
class BattleLogReplayAvailableMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23459; }
    const char* getMessageTypeName() const override { return "BattleLogReplayAvailableMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeLongLong(q136_);
        stream().writeLongLong(q144_);
        stream().writeVInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        q136_ = stream().readLongLong();
        q144_ = stream().readLongLong();
        v132_ = stream().readVInt();
    }
    i64 q136_ = 0, q144_ = 0;
    i32 v132_ = 0;
};

// ---- 14115 BattleLogViewReplayMessage ----
class BattleLogViewReplayMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14115; }
    const char* getMessageTypeName() const override { return "BattleLogViewReplayMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeLongLong(q136_);
        stream().writeLongLong(q144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        q136_ = stream().readLongLong();
        q144_ = stream().readLongLong();
    }
    i64 q136_ = 0, q144_ = 0;
};

// ---- 24413 BattleReportStreamMessage ----
class BattleReportStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24413; }
    const char* getMessageTypeName() const override { return "BattleReportStreamMessage"; }
    void encode() override;
    void decode() override;
    LogicLong ownerId_;
    std::vector<std::pair<i32, std::unique_ptr<BattleReportStreamEntry>>> entries_;
    bool isNull_ = false;
};

// ---- 10160 BillingCancelledByClientMessage ----
class BillingCancelledByClientMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10160; }
    const char* getMessageTypeName() const override { return "BillingCancelledByClientMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::string> productIds_;
    bool isNull_ = false;
};

// ---- 20121 BillingRequestFailedMessage ----
class BillingRequestFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20121; }
    const char* getMessageTypeName() const override { return "BillingRequestFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(errorCode_);
        stream().writeString(message_ ? &*message_ : nullptr);
        stream().writeInt(detail_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readInt();
        message_ = stream().readString();
        detail_ = stream().readInt();
    }
    i32 errorCode_ = 0, detail_ = 0;
    std::optional<std::string> message_;
};

// Bind* family: [force flag] + account strings (+ optional blobs/vint).
class BindAccountMessage : public PiranhaMessage {
public:
    void encodeBind();
    void decodeBind();
    bool hasForce_ = true;
    bool force_ = false;
    std::optional<std::string> accountId_, token_, extra_;
    std::vector<std::vector<u8>> blobs_;
    i32 vintTail_ = 0;
    bool hasVintTail_ = false;
    int blobCount_ = 0; // 0 or 3 depending on subclass
};

#define TITAN_BIND(Name, Id, Blobs, VintTail, Force)                               \
    class Name : public BindAccountMessage {                                       \
    public:                                                                        \
        int getMessageType() const override { return Id; }                         \
        const char* getMessageTypeName() const override { return #Name; }          \
        void encode() override {                                                   \
            PiranhaMessage::encode();                                              \
            blobCount_ = Blobs;                                                    \
            hasVintTail_ = VintTail;                                               \
            hasForce_ = Force;                                                     \
            encodeBind();                                                          \
        }                                                                          \
        void decode() override {                                                   \
            PiranhaMessage::decode();                                              \
            blobCount_ = Blobs;                                                    \
            hasVintTail_ = VintTail;                                               \
            hasForce_ = Force;                                                     \
            decodeBind();                                                          \
        }                                                                          \
    }

TITAN_BIND(BindFacebookAccountMessage, 14201, 0, false, true);
TITAN_BIND(BindGamecenterAccountMessage, 14212, 3, false, true);
TITAN_BIND(BindGoogleServiceAccountMessage, 14262, 0, false, true);
TITAN_BIND(BindKakaoAccountMessage, 14202, 0, false, true);
TITAN_BIND(BindLineAccountMessage, 14203, 0, false, true);
TITAN_BIND(BindTencentAccountMessage, 14266, 0, true, false);
TITAN_BIND(BindYoozooAccountMessage, 14266, 0, true, false);

#undef TITAN_BIND

// ---- 24700 BrawlTvChannelListMessage ----
class BrawlTvChannelListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24700; }
    const char* getMessageTypeName() const override { return "BrawlTvChannelListMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<BrawlTvChannelInfo>> channels_;
};

// ---- 24701 BrawlTvChannelNextUpMessage ----
class BrawlTvChannelNextUpMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24701; }
    const char* getMessageTypeName() const override { return "BrawlTvChannelNextUpMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
        stream().writeBoolean(hasBattle_);
        if (hasBattle_) battleId_.encode(stream());
        else stream().writeStringReference(stringRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
        hasBattle_ = stream().readBoolean();
        if (hasBattle_) battleId_ = LogicLong::decode(stream());
        else stringRef_ = stream().readStringReference();
    }
    i32 v132_ = 0, v136_ = 0;
    bool hasBattle_ = false;
    LogicLong battleId_;
    std::string stringRef_;
};

// ---- 28363 BuyBundleBillingPackResponseMessage: vint + vint ----
class BuyBundleBillingPackResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28363; }
    const char* getMessageTypeName() const override { return "BuyBundleBillingPackResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
    }
    i32 v132_ = 0, v136_ = 0;
};

// ---- 20154 CafeBazaarBillingProcessedByServerMessage ----
class CafeBazaarBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20154; }
    const char* getMessageTypeName() const override { return "CafeBazaarBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(receipt_ ? &*receipt_ : nullptr);
        stream().writeString(token_ ? &*token_ : nullptr);
        stream().writeInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        receipt_ = stream().readString();
        token_ = stream().readString();
        result_ = stream().readInt();
    }
    std::optional<std::string> receipt_, token_;
    i32 result_ = 0;
};

// ---- 10153 CafeBazaarBillingRequestMessage: 6 strings ----
class CafeBazaarBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10153; }
    const char* getMessageTypeName() const override { return "CafeBazaarBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&s1_, &s2_, &s3_, &s4_, &s5_, &s6_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
        s4_ = stream().readString();
        s5_ = stream().readString();
        s6_ = stream().readString();
    }
    std::optional<std::string> s1_, s2_, s3_, s4_, s5_, s6_;
};

// ---- 14106 CancelMatchmakingMessage: empty ----
class CancelMatchmakingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14106; }
    const char* getMessageTypeName() const override { return "CancelMatchmakingMessage"; }
};

} // namespace titan
