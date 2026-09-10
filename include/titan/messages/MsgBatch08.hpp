#pragma once

// Message batch 08 — reversed from libg_decrypted.so (ARM64).
// Classes: RankedMatchFinalPreparationStarted .. SCIDLogoutAllDevicesResult.
// (ResetAccountMessage already lives in titan/messages/ResetAccountMessage.hpp.)

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch04.hpp" // LogicClientAvatar
#include "titan/messages/MsgBatch06.hpp" // LogicCompressedString
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(LogicRankedMatchPlayer);
TITAN_PENDING_ENTRY(LogicRankedMatch);

// ---- 22158 RankedMatchFinalPreparationStartedMessage: vint ----
class RankedMatchFinalPreparationStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22158; }
    const char* getMessageTypeName() const override { return "RankedMatchFinalPreparationStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
    }
    i32 v_ = 0;
};

// ---- 22157 RankedMatchHeroDataUpdatedMessage ----
class RankedMatchHeroDataUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22157; }
    const char* getMessageTypeName() const override { return "RankedMatchHeroDataUpdatedMessage"; }
    void encode() override;
    void decode() override;
    bool hasPlayer_ = false;
    std::unique_ptr<LogicRankedMatchPlayer> player_;
};

// ---- 22156 RankedMatchHeroPickedMessage ----
class RankedMatchHeroPickedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22156; }
    const char* getMessageTypeName() const override { return "RankedMatchHeroPickedMessage"; }
    void encode() override;
    void decode() override;
    bool hasPlayer_ = false;
    std::unique_ptr<LogicRankedMatchPlayer> player_;
};

// ---- 22155 RankedMatchPickHeroFailedMessage ----
class RankedMatchPickHeroFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22155; }
    const char* getMessageTypeName() const override { return "RankedMatchPickHeroFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
        DataReference::encodeNullable(stream(), hero1_);
        stream().writeBoolean(flag_);
        DataReference::encodeNullable(stream(), hero2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
        hero1_ = DataReference::decodeNullable(stream());
        flag_ = stream().readBoolean();
        hero2_ = DataReference::decodeNullable(stream());
    }
    i32 v_ = 0;
    std::optional<DataReference> hero1_, hero2_;
    bool flag_ = false;
};

// ---- 12155 RankedMatchPickHeroMessage ----
class RankedMatchPickHeroMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12155; }
    const char* getMessageTypeName() const override { return "RankedMatchPickHeroMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), hero_);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        hero_ = DataReference::decodeNullable(stream());
        flag_ = stream().readBoolean();
    }
    std::optional<DataReference> hero_;
    bool flag_ = false;
};

// ---- 22154 RankedMatchPickStartedMessage ----
class RankedMatchPickStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22154; }
    const char* getMessageTypeName() const override { return "RankedMatchPickStartedMessage"; }
    void encode() override;
    void decode() override;
    bool hasPlayer_ = false;
    std::unique_ptr<LogicRankedMatchPlayer> player_;
    i32 v144_ = 0, v148_ = 0, v152_ = 0;
    LogicLong id156_;
};

// ---- 22150 RankedMatchStartedMessage ----
class RankedMatchStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22150; }
    const char* getMessageTypeName() const override { return "RankedMatchStartedMessage"; }
    void encode() override;
    void decode() override;
    bool hasMatch_ = false;
    std::unique_ptr<LogicRankedMatch> match_;
};

// ---- 22159 RankedMatchTerminatedMessage ----
class RankedMatchTerminatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22159; }
    const char* getMessageTypeName() const override { return "RankedMatchTerminatedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
        stream().writeVInt(static_cast<i32>(names_.size()));
        for (const auto& n : names_) stream().writeString(&n);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
        const i32 n = stream().readVInt();
        names_.clear();
        for (i32 i = 0; i < n; ++i) names_.push_back(stream().readString().value_or(""));
    }
    i32 v_ = 0;
    std::vector<std::string> names_;
};

// ---- 12157 RankedMatchUpdateHeroDataMessage: 3 datarefs ----
class RankedMatchUpdateHeroDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12157; }
    const char* getMessageTypeName() const override { return "RankedMatchUpdateHeroDataMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), ref1_);
        DataReference::encodeNullable(stream(), ref2_);
        DataReference::encodeNullable(stream(), ref3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        ref1_ = DataReference::decodeNullable(stream());
        ref2_ = DataReference::decodeNullable(stream());
        ref3_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> ref1_, ref2_, ref3_;
};

// ---- 14418 RemoveAvatarStreamEntryMessage: logiclong ----
class RemoveAvatarStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14418; }
    const char* getMessageTypeName() const override { return "RemoveAvatarStreamEntryMessage"; }
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

// ---- 10119 ReportAllianceStreamMessage: 2 logiclong ----
class ReportAllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10119; }
    const char* getMessageTypeName() const override { return "ReportAllianceStreamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        id1_.encode(stream());
        id2_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        id1_ = LogicLong::decode(stream());
        id2_ = LogicLong::decode(stream());
    }
    LogicLong id1_, id2_;
};

// ---- 12125 ReportPlayerMapMessage ----
class ReportPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12125; }
    const char* getMessageTypeName() const override { return "ReportPlayerMapMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        stream().writeLongLong(v144_);
        stream().writeLongLong(v152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        v144_ = stream().readLongLong();
        v152_ = stream().readLongLong();
    }
    LogicLong mapId_;
    i64 v144_ = 0, v152_ = 0;
};

// ---- 22125 ReportPlayerMapResponseMessage ----
class ReportPlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22125; }
    const char* getMessageTypeName() const override { return "ReportPlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        mapId_.encode(stream());
        stream().writeString(note_ ? &*note_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        note_ = stream().readString();
    }
    i32 result_ = 0;
    LogicLong mapId_;
    std::optional<std::string> note_;
};

// ---- 10117 ReportUserMessage ----
class ReportUserMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10117; }
    const char* getMessageTypeName() const override { return "ReportUserMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
        playerId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
        playerId_ = LogicLong::decode(stream());
    }
    i32 reason_ = 0;
    LogicLong playerId_;
};

// ---- 20117 ReportUserStatusMessage: int + int ----
class ReportUserStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20117; }
    const char* getMessageTypeName() const override { return "ReportUserStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(v1_);
        stream().writeInt(v2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readInt();
        v2_ = stream().readInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 14317 RequestJoinAllianceMessage ----
class RequestJoinAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14317; }
    const char* getMessageTypeName() const override { return "RequestJoinAllianceMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
        stream().writeString(message_ ? &*message_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
        message_ = stream().readString();
    }
    LogicLong allianceId_;
    std::optional<std::string> message_;
};

// ---- 19004 RequestLatencyTestStatusMessage: vint ----
class RequestLatencyTestStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19004; }
    const char* getMessageTypeName() const override { return "RequestLatencyTestStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
    }
    i32 v_ = 0;
};

// ---- 14117 RequestMatchCancelMessage: empty ----
class RequestMatchCancelMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14117; }
    const char* getMessageTypeName() const override { return "RequestMatchCancelMessage"; }
};

// ---- 14321 RespondToAllianceJoinRequestMessage ----
class RespondToAllianceJoinRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14321; }
    const char* getMessageTypeName() const override { return "RespondToAllianceJoinRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeBoolean(accept_);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        accept_ = stream().readBoolean();
    }
    LogicLong playerId_;
    bool accept_ = false;
};

// ---- 28689 SCIDAccountAlreadyBoundMessage ----
class SCIDAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28689; }
    const char* getMessageTypeName() const override { return "SCIDAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> token_, name_, extra_;
    bool hasPlayerId_ = false;
    LogicLong playerId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

// ---- 25165 SCIDAccountBoundMessage: int + compressed ----
class SCIDAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 25165; }
    const char* getMessageTypeName() const override { return "SCIDAccountBoundMessage"; }
    void encode() override;
    void decode() override;
    i32 result_ = 0;
    std::unique_ptr<LogicCompressedString> data_;
};

// ---- 10636 SCIDBindAccountMessage: 2 compressed ----
class SCIDBindAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10636; }
    const char* getMessageTypeName() const override { return "SCIDBindAccountMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCompressedString> f1_, f2_;
};

// ---- 11736 SCIDLogoutAllDevicesMessage: compressed ----
class SCIDLogoutAllDevicesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 11736; }
    const char* getMessageTypeName() const override { return "SCIDLogoutAllDevicesMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCompressedString> token_;
};

// ---- 23067 SCIDLogoutAllDevicesResultMessage: vint ----
class SCIDLogoutAllDevicesResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23067; }
    const char* getMessageTypeName() const override { return "SCIDLogoutAllDevicesResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
    }
    i32 result_ = 0;
};

} // namespace titan
