#pragma once

// Message batch 04 — reversed from libg_decrypted.so (ARM64).
// Classes: DebugNewbieCoopOverrideSet .. GetTokenFriendResult.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/game/LogicClientAvatar.hpp" // real LogicClientAvatar/LogicDataSlot
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(FriendEntry);
TITAN_PENDING_ENTRY(FriendOnlineStatus);
TITAN_PENDING_ENTRY(FriendSuggestionEntry);
TITAN_PENDING_ENTRY(BattleLogPlayerEntry);

// ---- 24116 DebugNewbieCoopOverrideSetMessage: int ----
class DebugNewbieCoopOverrideSetMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24116; }
    const char* getMessageTypeName() const override { return "DebugNewbieCoopOverrideSetMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(value_);
    }
    void decode() override {
        PiranhaMessage::decode();
        value_ = stream().readInt();
    }
    i32 value_ = 0;
};

// ---- 22109 DebugPlayerMapReviewResultOverrideSetMessage: vint ----
class DebugPlayerMapReviewResultOverrideSetMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22109; }
    const char* getMessageTypeName() const override { return "DebugPlayerMapReviewResultOverrideSetMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(value_);
    }
    void decode() override {
        PiranhaMessage::decode();
        value_ = stream().readVInt();
    }
    i32 value_ = 0;
};

// ---- 12101 DeletePlayerMapMessage: logiclong ----
class DeletePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12101; }
    const char* getMessageTypeName() const override { return "DeletePlayerMapMessage"; }
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

// ---- 22101 DeletePlayerMapResponseMessage ----
class DeletePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22101; }
    const char* getMessageTypeName() const override { return "DeletePlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        mapId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
    }
    i32 result_ = 0;
    LogicLong mapId_;
};

// ---- 25892 DisconnectedMessage: vint ----
class DisconnectedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 25892; }
    const char* getMessageTypeName() const override { return "DisconnectedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readVInt();
    }
    i32 reason_ = 0;
};

// Forward declared in batch 01; LogicCommand lives there.
class LogicCommand;

// ---- 14102 EndClientTurnMessage ----
class EndClientTurnMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14102; }
    const char* getMessageTypeName() const override { return "EndClientTurnMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    i32 tick_ = 0, checksum_ = 0;
    std::vector<std::unique_ptr<LogicCommand>> commands_;
    std::optional<std::vector<u8>> payload_; // bytes, -1=null
};

// ---- 12107 EnterMapEditorMessage: empty ----
class EnterMapEditorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12107; }
    const char* getMessageTypeName() const override { return "EnterMapEditorMessage"; }
};

// Shared shape for *AccountAlreadyBound: string id, bool hasAvatar,
// [logiclong], string name, [bool flag], LogicClientAvatar, [string extra].
class AccountAlreadyBoundBase : public PiranhaMessage {
public:
    void encodeBound(bool hasFlag, bool hasExtra);
    void decodeBound(bool hasFlag, bool hasExtra);
    std::optional<std::string> accountId_, avatarName_, extra_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    bool flag_ = false;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

// ---- 24202 FacebookAccountAlreadyBoundMessage ----
class FacebookAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 24202; }
    const char* getMessageTypeName() const override { return "FacebookAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(false, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(false, false);
    }
};

// ---- 24201 FacebookAccountBoundMessage: int ----
class FacebookAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24201; }
    const char* getMessageTypeName() const override { return "FacebookAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readInt();
    }
    i32 result_ = 0;
};

// ---- 24214 FacebookAccountUnboundMessage: empty ----
class FacebookAccountUnboundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24214; }
    const char* getMessageTypeName() const override { return "FacebookAccountUnboundMessage"; }
};

// ---- 20105 FriendListMessage ----
class FriendListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20105; }
    const char* getMessageTypeName() const override { return "FriendListMessage"; }
    void encode() override;
    void decode() override;
    i32 unknown_ = 0;
    bool flag1_ = false, flag2_ = false;
    std::vector<std::unique_ptr<FriendEntry>> friends_;
    bool isNull_ = false; // -> writeInt(-1)
};

// ---- 20106 FriendListUpdateMessage ----
class FriendListUpdateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20106; }
    const char* getMessageTypeName() const override { return "FriendListUpdateMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    std::unique_ptr<FriendEntry> entry_;
};

// ---- 24555 FriendOnlineStatusEntryMessage ----
class FriendOnlineStatusEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24555; }
    const char* getMessageTypeName() const override { return "FriendOnlineStatusEntryMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    bool hasStatus_ = false;
    std::unique_ptr<FriendOnlineStatus> status_;
};

// ---- 20109 FriendOnlineStatusMessage: count(-1=null) + logiclongs ----
class FriendOnlineStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20109; }
    const char* getMessageTypeName() const override { return "FriendOnlineStatusMessage"; }
    void encode() override;
    void decode() override;
    std::vector<LogicLong> avatarIds_;
    bool isNull_ = false;
};

// ---- 20199 FriendSuggestionsMessage ----
class FriendSuggestionsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20199; }
    const char* getMessageTypeName() const override { return "FriendSuggestionsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<FriendSuggestionEntry>> suggestions_;
};

// ---- 22687 GamecenterAccountAlreadyBoundMessage (+flag+extra) ----
class GamecenterAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 22687; }
    const char* getMessageTypeName() const override { return "GamecenterAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(true, true);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(true, true);
    }
};

// ---- 26085 GamecenterAccountBoundMessage: int ----
class GamecenterAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26085; }
    const char* getMessageTypeName() const override { return "GamecenterAccountBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readInt();
    }
    i32 result_ = 0;
};

// ---- 23302 GetAllianceInviteTokenResultMessage ----
class GetAllianceInviteTokenResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23302; }
    const char* getMessageTypeName() const override { return "GetAllianceInviteTokenResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        token_ = stream().readString();
    }
    i32 result_ = 0;
    std::optional<std::string> token_;
};

// ---- 14114 GetBattleLogMessage: empty ----
class GetBattleLogMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14114; }
    const char* getMessageTypeName() const override { return "GetBattleLogMessage"; }
};

// ---- 12905 GetCurrentBattleReplayDataMessage: empty ----
class GetCurrentBattleReplayDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12905; }
    const char* getMessageTypeName() const override { return "GetCurrentBattleReplayDataMessage"; }
};

// ---- 14403 GetLeaderboardMessage ----
class GetLeaderboardMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14403; }
    const char* getMessageTypeName() const override { return "GetLeaderboardMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(flag_);
        stream().writeVInt(leaderboardType_);
        DataReference::encodeNullable(stream(), leaderboard_);
        stream().writeVInt(unknown_);
    }
    void decode() override {
        PiranhaMessage::decode();
        flag_ = stream().readBoolean();
        leaderboardType_ = stream().readVInt();
        leaderboard_ = DataReference::decodeNullable(stream());
        unknown_ = stream().readVInt();
    }
    bool flag_ = false;
    i32 leaderboardType_ = 0, unknown_ = 0;
    std::optional<DataReference> leaderboard_;
};

// ---- 12102 GetPlayerMapsMessage: empty ----
class GetPlayerMapsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12102; }
    const char* getMessageTypeName() const override { return "GetPlayerMapsMessage"; }
};

// ---- 14113 GetPlayerProfileMessage ----
class GetPlayerProfileMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14113; }
    const char* getMessageTypeName() const override { return "GetPlayerProfileMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    bool hasEntry_ = false;
    std::unique_ptr<BattleLogPlayerEntry> entry_;
    i32 unknown_ = 0;
};

// ---- 14277 GetSeasonRewardsMessage: vint ----
class GetSeasonRewardsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14277; }
    const char* getMessageTypeName() const override { return "GetSeasonRewardsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(season_);
    }
    void decode() override {
        PiranhaMessage::decode();
        season_ = stream().readVInt();
    }
    i32 season_ = 0;
};

// ---- 22089 GetTokenFriendResultMessage ----
class GetTokenFriendResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22089; }
    const char* getMessageTypeName() const override { return "GetTokenFriendResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        token_ = stream().readString();
    }
    i32 result_ = 0;
    std::optional<std::string> token_;
};

} // namespace titan
