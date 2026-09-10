#pragma once

// Message batch 10 — reversed from libg_decrypted.so (ARM64).
// Classes: StartLatencyTestRequest .. TeamKick.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(LatencyTestConfiguration);
TITAN_PENDING_ENTRY(LogicPlayer);
TITAN_PENDING_ENTRY(LogicVector2);
TITAN_PENDING_ENTRY(BattlePlayerMap);
TITAN_PENDING_ENTRY(TeamEntry);
TITAN_PENDING_ENTRY(TeamInvitation);
TITAN_PENDING_ENTRY(TeamInvitationDataEntry);

// ---- 29001 StartLatencyTestRequestMessage ----
class StartLatencyTestRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29001; }
    const char* getMessageTypeName() const override { return "StartLatencyTestRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!config_) throw pending_reverse("StartLatencyTestRequestMessage needs LatencyTestConfiguration");
        config_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        config_ = std::make_unique<LatencyTestConfiguration>();
        config_->decode(stream());
    }
    std::unique_ptr<LatencyTestConfiguration> config_;
};

// ---- 20559 StartLoadingMessage ----
class StartLoadingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20559; }
    const char* getMessageTypeName() const override { return "StartLoadingMessage"; }
    void encode() override;
    void decode() override;
    i32 head_[3] = {}; // unk132..140
    std::vector<std::unique_ptr<LogicPlayer>> players_;   // int count
    std::vector<std::unique_ptr<LogicVector2>> spawns_;   // int count
    std::vector<i32> unkInts_;                            // int count
    i32 unk144_ = 0;
    i32 vints_[5] = {}; // unk148..164 (vint)
    // NOTE: spec order lists vint unk168 between unk164 and mapRef.
    i32 v168_ = 0;
    std::optional<DataReference> mapRef_;
    std::unique_ptr<BattlePlayerMap> battleMap_;
    bool unk172_ = false, unk173_ = false;
    i32 v176_ = 0, v180_ = 0;
};

// ---- 14104 StartMissionMessage: empty ----
// NOTE: same numeric id as StartSpectateMessage (14104); flagged re-verify.
class StartMissionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14104; }
    const char* getMessageTypeName() const override { return "StartMissionMessage"; }
};

// ---- 14104 StartSpectateMessage ----
// NOTE: same numeric id as StartMissionMessage (14104); flagged re-verify.
class StartSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14104; }
    const char* getMessageTypeName() const override { return "StartSpectateMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeBoolean(allowSpectate_);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        allowSpectate_ = stream().readBoolean();
    }
    LogicLong playerId_;
    bool allowSpectate_ = false;
};

// ---- 24106 StopHomeLogicMessage: empty ----
class StopHomeLogicMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24106; }
    const char* getMessageTypeName() const override { return "StopHomeLogicMessage"; }
};

// ---- 14107 StopSpectateMessage: empty ----
class StopSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14107; }
    const char* getMessageTypeName() const override { return "StopSpectateMessage"; }
};

// ---- 12104 SubmitPlayerMapMessage: logiclong ----
class SubmitPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12104; }
    const char* getMessageTypeName() const override { return "SubmitPlayerMapMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
    }
    LogicLong mapId_;
};

// ---- 22104 SubmitPlayerMapResponseMessage ----
class SubmitPlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22104; }
    const char* getMessageTypeName() const override { return "SubmitPlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(resultCode_);
        mapId_.encode(stream());
        stream().writeVInt(unk144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        unk144_ = stream().readVInt();
    }
    i32 resultCode_ = 0, unk144_ = 0;
    LogicLong mapId_;
};

// ---- 29900 SupercellIdNotificationMessage: compressed payload ----
class SupercellIdNotificationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29900; }
    const char* getMessageTypeName() const override { return "SupercellIdNotificationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(payload_ ? &*payload_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        payload_ = stream().readString();
    }
    std::optional<std::string> payload_;
};

// ---- 14370 TeamAllianceMemberInviteMessage: 3 vint ----
class TeamAllianceMemberInviteMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14370; }
    const char* getMessageTypeName() const override { return "TeamAllianceMemberInviteMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(slotId_);
        stream().writeVInt(clubIdHi_);
        stream().writeVInt(clubIdLo_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotId_ = stream().readVInt();
        clubIdHi_ = stream().readVInt();
        clubIdLo_ = stream().readVInt();
    }
    i32 slotId_ = 0, clubIdHi_ = 0, clubIdLo_ = 0;
};

// ---- 14373 TeamBotSlotDisableMessage ----
class TeamBotSlotDisableMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14373; }
    const char* getMessageTypeName() const override { return "TeamBotSlotDisableMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(slotIndex_);
        stream().writeBoolean(disabled_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotIndex_ = stream().readInt();
        disabled_ = stream().readBoolean();
    }
    i32 slotIndex_ = 0;
    bool disabled_ = false;
};

// ---- 14354 TeamChangeMemberSettingsMessage: 2 datarefs ----
class TeamChangeMemberSettingsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14354; }
    const char* getMessageTypeName() const override { return "TeamChangeMemberSettingsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), charRef_);
        DataReference::encodeNullable(stream(), skinRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        charRef_ = DataReference::decodeNullable(stream());
        skinRef_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> charRef_, skinRef_;
};

// ---- 14359 TeamChatMessage: string ----
class TeamChatMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14359; }
    const char* getMessageTypeName() const override { return "TeamChatMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(text_ ? &*text_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        text_ = stream().readString();
    }
    std::optional<std::string> text_;
};

// ---- 14367 TeamClearInviteMessage: logiclong ----
class TeamClearInviteMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14367; }
    const char* getMessageTypeName() const override { return "TeamClearInviteMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        inviteId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        inviteId_ = LogicLong::decode(stream());
    }
    LogicLong inviteId_;
};

// ---- 14350 TeamCreateMessage ----
class TeamCreateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14350; }
    const char* getMessageTypeName() const override { return "TeamCreateMessage"; }
    void encode() override;
    void decode() override;
    i32 v136_ = 0, v140_ = 0, teamType_ = 0;
    bool unk144_ = false, hasInvite_ = false;
    LogicLong inviteId_;
    i32 unk160_ = 0;
    std::optional<DataReference> eventRef_;
    LogicLong roomId_;
};

// ---- 24129 TeamErrorMessage: vint + vint ----
class TeamErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24129; }
    const char* getMessageTypeName() const override { return "TeamErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(errorCode_);
        stream().writeVInt(unk136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readVInt();
        unk136_ = stream().readVInt();
    }
    i32 errorCode_ = 0, unk136_ = 0;
};

// ---- 24130 TeamGameStartingMessage: 3 vint ----
class TeamGameStartingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24130; }
    const char* getMessageTypeName() const override { return "TeamGameStartingMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(timerSec_);
        stream().writeVInt(unk134_);
        stream().writeVInt(unk135_);
    }
    void decode() override {
        PiranhaMessage::decode();
        timerSec_ = stream().readVInt();
        unk134_ = stream().readVInt();
        unk135_ = stream().readVInt();
    }
    i32 timerSec_ = 0, unk134_ = 0, unk135_ = 0;
};

// ---- 24589 TeamInvitationMessage ----
class TeamInvitationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24589; }
    const char* getMessageTypeName() const override { return "TeamInvitationMessage"; }
    void encode() override;
    void decode() override;
    i32 slotId_ = 0;
    std::unique_ptr<TeamInvitation> invitation_;
};

// ---- 14479 TeamInvitationResponseMessage ----
class TeamInvitationResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14479; }
    const char* getMessageTypeName() const override { return "TeamInvitationResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(response_);
        inviterId_.encode(stream());
        stream().writeBoolean(accepted_);
    }
    void decode() override {
        PiranhaMessage::decode();
        response_ = stream().readVInt();
        inviterId_ = LogicLong::decode(stream());
        accepted_ = stream().readBoolean();
    }
    i32 response_ = 0;
    LogicLong inviterId_;
    bool accepted_ = false;
};

// ---- 14365 TeamInviteMessage: 3 vint ----
class TeamInviteMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14365; }
    const char* getMessageTypeName() const override { return "TeamInviteMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(slotId_);
        stream().writeVInt(unk134_);
        stream().writeVInt(unk135_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotId_ = stream().readVInt();
        unk134_ = stream().readVInt();
        unk135_ = stream().readVInt();
    }
    i32 slotId_ = 0, unk134_ = 0, unk135_ = 0;
};

// ---- 14368 TeamInviteResponseMessage: vint + vint ----
class TeamInviteResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14368; }
    const char* getMessageTypeName() const override { return "TeamInviteResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(response_);
        stream().writeVInt(unk136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        response_ = stream().readVInt();
        unk136_ = stream().readVInt();
    }
    i32 response_ = 0, unk136_ = 0;
};

// ---- 24582 TeamInviteStatusMessage ----
// Encode always writes the entry; decode reads a bool flag first (DIFF in
// binary directions). We write flag=true + entry; decode honors the flag.
// Flagged for verify against a live capture.
class TeamInviteStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24582; }
    const char* getMessageTypeName() const override { return "TeamInviteStatusMessage"; }
    void encode() override;
    void decode() override;
    i32 statusCode_ = 0;
    std::unique_ptr<TeamInvitationDataEntry> entry_;
};

// ---- 14351 TeamJoinMessage ----
class TeamJoinMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14351; }
    const char* getMessageTypeName() const override { return "TeamJoinMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v136_);
        stream().writeVInt(v140_);
        stream().writeVInt(slotId_);
        DataReference::encodeNullable(stream(), eventRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v136_ = stream().readVInt();
        v140_ = stream().readVInt();
        slotId_ = stream().readVInt();
        eventRef_ = DataReference::decodeNullable(stream());
    }
    i32 v136_ = 0, v140_ = 0, slotId_ = 0;
    std::optional<DataReference> eventRef_;
};

// ---- 14371 TeamJoinOrCreateRoomMessage ----
class TeamJoinOrCreateRoomMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14371; }
    const char* getMessageTypeName() const override { return "TeamJoinOrCreateRoomMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(roomCode_);
        stream().writeBoolean(create_);
        stream().writeInt(unk156_);
        DataReference::encodeNullable(stream(), eventRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        roomCode_ = stream().readStringReference();
        create_ = stream().readBoolean();
        unk156_ = stream().readInt();
        eventRef_ = DataReference::decodeNullable(stream());
    }
    std::string roomCode_;
    bool create_ = false;
    i32 unk156_ = 0;
    std::optional<DataReference> eventRef_;
};

// ---- 14352 TeamKickMessage: vint + vint ----
class TeamKickMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14352; }
    const char* getMessageTypeName() const override { return "TeamKickMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(slotId_);
        stream().writeVInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotId_ = stream().readVInt();
        reason_ = stream().readVInt();
    }
    i32 slotId_ = 0, reason_ = 0;
};

} // namespace titan
