#pragma once

// Message batch 11 — reversed from libg_decrypted.so (ARM64).
// Classes: TeamLeave .. TencentBillingProcessed.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch00.hpp" // StreamEntry
#include "titan/messages/MsgBatch04.hpp" // LogicClientAvatar
#include "titan/messages/MsgBatch10.hpp" // TeamEntry
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(LogicTencentAntiAddictionInstruction);

// ---- 14353 TeamLeaveMessage: empty ----
class TeamLeaveMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14353; }
    const char* getMessageTypeName() const override { return "TeamLeaveMessage"; }
};

// ---- 24125 TeamLeftMessage: int ----
class TeamLeftMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24125; }
    const char* getMessageTypeName() const override { return "TeamLeftMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readInt();
    }
    i32 v132_ = 0;
};

// ---- 14361 TeamMemberStatusMessage: vint ----
class TeamMemberStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14361; }
    const char* getMessageTypeName() const override { return "TeamMemberStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
    }
    i32 v132_ = 0;
};

// ---- 24124 TeamMessage: TeamEntry ----
class TeamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24124; }
    const char* getMessageTypeName() const override { return "TeamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!team_) throw pending_reverse("TeamMessage needs TeamEntry");
        team_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        team_ = std::make_unique<TeamEntry>();
        team_->decode(stream());
    }
    std::unique_ptr<TeamEntry> team_;
};

// ---- 14360 TeamPostAdMessage: vint ----
class TeamPostAdMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14360; }
    const char* getMessageTypeName() const override { return "TeamPostAdMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
    }
    i32 v132_ = 0;
};

// ---- 14369 TeamPremadeChatMessage ----
class TeamPremadeChatMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14369; }
    const char* getMessageTypeName() const override { return "TeamPremadeChatMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), dataRef_);
        stream().writeBoolean(hasPlayerId_);
        if (hasPlayerId_) playerId_.encode(stream());
        stream().writeVInt(v156_);
        stream().writeVInt(v152_);
        stream().writeVInt(v160_);
    }
    void decode() override {
        PiranhaMessage::decode();
        dataRef_ = DataReference::decodeNullable(stream());
        hasPlayerId_ = stream().readBoolean();
        if (hasPlayerId_) playerId_ = LogicLong::decode(stream());
        v156_ = stream().readVInt();
        v152_ = stream().readVInt();
        hasV160_ = !stream().isAtEnd();
        if (hasV160_) v160_ = stream().readVInt();
    }
    std::optional<DataReference> dataRef_;
    bool hasPlayerId_ = false;
    LogicLong playerId_;
    i32 v156_ = 0, v152_ = 0, v160_ = 0;
    bool hasV160_ = false;
};

// ---- 14364 TeamReportChatMessage: 2 logiclong ----
class TeamReportChatMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14364; }
    const char* getMessageTypeName() const override { return "TeamReportChatMessage"; }
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

// ---- 14882 TeamRequestJoinApproveMessage ----
class TeamRequestJoinApproveMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14882; }
    const char* getMessageTypeName() const override { return "TeamRequestJoinApproveMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeBoolean(v144_);
        stream().writeBoolean(v145_);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        v144_ = stream().readBoolean();
        v145_ = stream().readBoolean();
    }
    LogicLong playerId_;
    bool v144_ = false, v145_ = false;
};

// ---- 14880 TeamRequestJoinCancelMessage: empty ----
class TeamRequestJoinCancelMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14880; }
    const char* getMessageTypeName() const override { return "TeamRequestJoinCancelMessage"; }
};

// ---- 14881 TeamRequestJoinMessage: 2 logiclong ----
class TeamRequestJoinMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14881; }
    const char* getMessageTypeName() const override { return "TeamRequestJoinMessage"; }
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

// ---- 14362 TeamSetEventMessage: vint + vint ----
class TeamSetEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14362; }
    const char* getMessageTypeName() const override { return "TeamSetEventMessage"; }
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

// ---- 14363 TeamSetLocationMessage ----
class TeamSetLocationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14363; }
    const char* getMessageTypeName() const override { return "TeamSetLocationMessage"; }
    void encode() override;
    void decode() override;
    std::optional<DataReference> locationRef_;
    i32 v144_ = 0;
    std::vector<i32> intList_; // vint count + vint items (verify on capture)
};

// ---- 14355 TeamSetMemberReadyMessage ----
class TeamSetMemberReadyMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14355; }
    const char* getMessageTypeName() const override { return "TeamSetMemberReadyMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(isReady_);
        stream().writeVInt(v136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        isReady_ = stream().readBoolean();
        v136_ = stream().readVInt();
    }
    bool isReady_ = false;
    i32 v136_ = 0;
};

// ---- 12110 TeamSetPlayerMapMessage ----
class TeamSetPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12110; }
    const char* getMessageTypeName() const override { return "TeamSetPlayerMapMessage"; }
    void encode() override;
    void decode() override;
    LogicLong mapId_;
    std::vector<i32> intList_; // vint count + vint items (verify on capture)
};

// ---- 14358 TeamSpectateMessage: 3 vint ----
class TeamSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14358; }
    const char* getMessageTypeName() const override { return "TeamSpectateMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
        stream().writeVInt(v140_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
        v140_ = stream().readVInt();
    }
    i32 v132_ = 0, v136_ = 0, v140_ = 0;
};

// ---- 24319 TeamStreamEntryRemovedMessage: 2 logiclong ----
class TeamStreamEntryRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24319; }
    const char* getMessageTypeName() const override { return "TeamStreamEntryRemovedMessage"; }
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

// ---- 24131 TeamStreamMessage (StreamEntry factory) ----
class TeamStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24131; }
    const char* getMessageTypeName() const override { return "TeamStreamMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0, v136_ = 0;
    std::vector<std::pair<i32, std::unique_ptr<StreamEntry>>> entries_;
    bool isNull_ = false;
};

// ---- 14357 TeamToggleMemberSideMessage ----
class TeamToggleMemberSideMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14357; }
    const char* getMessageTypeName() const override { return "TeamToggleMemberSideMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(static_cast<i32>(memberIds_.size()));
        for (const auto& id : memberIds_) id.encode(stream());
        stream().writeVInt(v152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        const i32 n = stream().readVInt();
        memberIds_.clear();
        for (i32 i = 0; i < n; ++i) memberIds_.push_back(LogicLong::decode(stream()));
        v152_ = stream().readVInt();
    }
    std::vector<LogicLong> memberIds_;
    i32 v152_ = 0;
};

// ---- 14356 TeamTogglePractiseMessage: empty ----
class TeamTogglePractiseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14356; }
    const char* getMessageTypeName() const override { return "TeamTogglePractiseMessage"; }
};

// ---- 14372 TeamToggleSettingsMessage: bool ----
class TeamToggleSettingsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14372; }
    const char* getMessageTypeName() const override { return "TeamToggleSettingsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readBoolean();
    }
    bool v132_ = false;
};

// ---- 24221 TencentAccountAlreadyBoundMessage ----
class TencentAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24221; }
    const char* getMessageTypeName() const override { return "TencentAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> id_, name_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

// ---- 24220 TencentAccountBoundMessage: int ----
class TencentAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24220; }
    const char* getMessageTypeName() const override { return "TencentAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readInt();
    }
    i32 v132_ = 0;
};

// ---- 14267 TencentAntiAddictionInstructionExecutedMessage ----
class TencentAntiAddictionInstructionExecutedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14267; }
    const char* getMessageTypeName() const override { return "TencentAntiAddictionInstructionExecutedMessage"; }
    void encode() override;
    void decode() override; // decode caps args at 11
    std::string name_;
    std::vector<std::string> args_;
    bool isNull_ = false;
};

// ---- 24222 TencentAntiAddictionInstructionMessage ----
class TencentAntiAddictionInstructionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24222; }
    const char* getMessageTypeName() const override { return "TencentAntiAddictionInstructionMessage"; }
    void encode() override;
    void decode() override;
    std::string name_;
    std::vector<std::unique_ptr<LogicTencentAntiAddictionInstruction>> instructions_;
    bool isNull_ = false;
};

// ---- 20153 TencentBillingProcessedByServerMessage ----
class TencentBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20153; }
    const char* getMessageTypeName() const override { return "TencentBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
        stream().writeStringReference(f152_);
        stream().writeInt(f168_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
        f152_ = stream().readStringReference();
        f168_ = stream().readInt();
    }
    std::string f136_, f152_;
    i32 f168_ = 0;
};

} // namespace titan
