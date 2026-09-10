#pragma once

// Message batch 05 — reversed from libg_decrypted.so (ARM64).
// Classes: GlobalChatLine .. KunlunBillingProcessed.
// (KeepAliveMessage/KeepAliveServerMessage already live in
// titan/messages/KeepAliveMessages.hpp and are skipped here.)

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch00.hpp" // AllianceHeaderEntry
#include "titan/messages/MsgBatch01.hpp" // LogicCommand
#include "titan/messages/MsgBatch04.hpp" // LogicClientAvatar, AccountAlreadyBoundBase
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

// ---- 24715 GlobalChatLineMessage ----
class GlobalChatLineMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24715; }
    const char* getMessageTypeName() const override { return "GlobalChatLineMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(message_ ? &*message_ : nullptr);
        stream().writeString(sender_ ? &*sender_ : nullptr);
        stream().writeVInt(senderLevel_);
        playerId_.encode(stream());
        targetId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        message_ = stream().readString();
        sender_ = stream().readString();
        senderLevel_ = stream().readVInt();
        playerId_ = LogicLong::decode(stream());
        targetId_ = LogicLong::decode(stream());
    }
    std::optional<std::string> message_, sender_;
    i32 senderLevel_ = 0;
    LogicLong playerId_, targetId_;
};

// ---- 12108 GoHomeFromMapEditorMessage: empty ----
class GoHomeFromMapEditorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12108; }
    const char* getMessageTypeName() const override { return "GoHomeFromMapEditorMessage"; }
};

// ---- 14109 GoHomeFromOfflinePractiseMessage: bool ----
class GoHomeFromOfflinePractiseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14109; }
    const char* getMessageTypeName() const override { return "GoHomeFromOfflinePractiseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(unknown_);
    }
    void decode() override {
        PiranhaMessage::decode();
        unknown_ = stream().readBoolean();
    }
    bool unknown_ = false;
};

// ---- 14108 GoHomeFromSpectateMessage: empty ----
class GoHomeFromSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14108; }
    const char* getMessageTypeName() const override { return "GoHomeFromSpectateMessage"; }
};

// ---- 14101 GoHomeMessage: bool + bool ----
class GoHomeMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14101; }
    const char* getMessageTypeName() const override { return "GoHomeMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(flagA_);
        stream().writeBoolean(flagB_);
    }
    void decode() override {
        PiranhaMessage::decode();
        flagA_ = stream().readBoolean();
        flagB_ = stream().readBoolean();
    }
    bool flagA_ = false, flagB_ = false;
};

// ---- 20152 GoogleBillingProcessedByServerMessage ----
class GoogleBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20152; }
    const char* getMessageTypeName() const override { return "GoogleBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(receipt_ ? &*receipt_ : nullptr);
        stream().writeString(signature_ ? &*signature_ : nullptr);
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        receipt_ = stream().readString();
        signature_ = stream().readString();
        resultCode_ = stream().readInt();
    }
    std::optional<std::string> receipt_, signature_;
    i32 resultCode_ = 0;
};

// ---- 10151 GoogleBillingRequestMessage: 6 strings ----
class GoogleBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10151; }
    const char* getMessageTypeName() const override { return "GoogleBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&f1_, &f2_, &f3_, &f4_, &f5_, &f6_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readString();
        f2_ = stream().readString();
        f3_ = stream().readString();
        f4_ = stream().readString();
        f5_ = stream().readString();
        f6_ = stream().readString();
    }
    std::optional<std::string> f1_, f2_, f3_, f4_, f5_, f6_;
};

// Google/Kakao already-bound share the AccountAlreadyBoundBase shape;
// Google has an extra bool flag before the avatar, Kakao does not.
class GoogleServiceAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 23494; }
    const char* getMessageTypeName() const override { return "GoogleServiceAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(true, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(true, false);
    }
};

// ---- 22377 GoogleServiceAccountBoundMessage: int ----
class GoogleServiceAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22377; }
    const char* getMessageTypeName() const override { return "GoogleServiceAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readInt();
    }
    i32 resultCode_ = 0;
};

// ---- 24114 HomeBattleReplayDataMessage ----
class HomeBattleReplayDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24114; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayDataMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(replaySlot_);
        if (replayData_) {
            const auto& p = *replayData_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        replaySlot_ = stream().readVInt();
        replayData_ = stream().readBytesNullable();
    }
    i32 replaySlot_ = 0;
    std::optional<std::vector<u8>> replayData_;
};

// ---- 24116 HomeBattleReplayFailedMessage: int ----
// NOTE: same numeric id as DebugNewbieCoopOverrideSetMessage (both 24116
// in this build); flagged for re-verify.
class HomeBattleReplayFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24116; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
    }
    i32 reason_ = 0;
};

// ---- 14114 HomeBattleReplayMessage ----
class HomeBattleReplayMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14114; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(replaySlot_);
        sharerId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        replaySlot_ = stream().readInt();
        sharerId_ = LogicLong::decode(stream());
    }
    i32 replaySlot_ = 0;
    LogicLong sharerId_;
};

// ---- 24117 HomeBattleReplayViewedMessage ----
class HomeBattleReplayViewedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24117; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayViewedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(replaySlot_);
        playerId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        replaySlot_ = stream().readVInt();
        playerId_ = LogicLong::decode(stream());
    }
    i32 replaySlot_ = 0;
    LogicLong playerId_;
};

// ---- 14105 HomeLogicStoppedMessage: vint tick/unknown + commands ----
class HomeLogicStoppedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14105; }
    const char* getMessageTypeName() const override { return "HomeLogicStoppedMessage"; }
    void encode() override;
    void decode() override; // decode caps count at 0x201 per binary
    i32 tick_ = 0, unknown_ = 0;
    std::vector<std::unique_ptr<LogicCommand>> commands_;
};

// ---- 14305 JoinAllianceMessage: logiclong ----
class JoinAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14305; }
    const char* getMessageTypeName() const override { return "JoinAllianceMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
    }
    LogicLong allianceId_;
};

// ---- 14323 JoinAllianceUsingInvitationMessage: logiclong ----
class JoinAllianceUsingInvitationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14323; }
    const char* getMessageTypeName() const override { return "JoinAllianceUsingInvitationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
    }
    LogicLong allianceId_;
};

// ---- 17190 JoinAllianceUsingTokenMessage ----
class JoinAllianceUsingTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17190; }
    const char* getMessageTypeName() const override { return "JoinAllianceUsingTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
        token_ = stream().readString();
    }
    LogicLong allianceId_;
    std::optional<std::string> token_;
};

// ---- 24304 JoinableAllianceListMessage (reuses AllianceHeaderEntry) ----
class JoinableAllianceListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24304; }
    const char* getMessageTypeName() const override { return "JoinableAllianceListMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<AllianceHeaderEntry>> headers_;
    bool isNull_ = false;
};

// ---- 24204 KakaoAccountAlreadyBoundMessage (no flag) ----
class KakaoAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 24204; }
    const char* getMessageTypeName() const override { return "KakaoAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(false, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(false, false);
    }
};

// ---- 24203 KakaoAccountBoundMessage: int ----
class KakaoAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24203; }
    const char* getMessageTypeName() const override { return "KakaoAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readInt();
    }
    i32 resultCode_ = 0;
};

// ---- 24215 KakaoAccountUnboundMessage: empty ----
class KakaoAccountUnboundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24215; }
    const char* getMessageTypeName() const override { return "KakaoAccountUnboundMessage"; }
};

// ---- 14307 KickAllianceMemberMessage ----
class KickAllianceMemberMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14307; }
    const char* getMessageTypeName() const override { return "KickAllianceMemberMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        name_ = stream().readString();
    }
    LogicLong playerId_;
    std::optional<std::string> name_;
};

// ---- 20156 KunlunBillingProcessedByServerMessage ----
class KunlunBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20156; }
    const char* getMessageTypeName() const override { return "KunlunBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(receipt_);
        stream().writeStringReference(signature_);
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        receipt_ = stream().readStringReference();
        signature_ = stream().readStringReference();
        resultCode_ = stream().readInt();
    }
    std::string receipt_, signature_;
    i32 resultCode_ = 0;
};

} // namespace titan
