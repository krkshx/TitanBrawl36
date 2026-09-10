#pragma once

// Message batch 07 — reversed from libg_decrypted.so (ARM64).
// Classes: MapPreview .. RankedMatchBanStarted.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch00.hpp" // AllianceHeaderEntry
#include "titan/messages/MsgBatch02.hpp" // PlayAgainStatus
#include "titan/messages/MsgBatch03.hpp" // LogicPlayerMap
#include "titan/messages/MsgBatch04.hpp" // LogicClientAvatar
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(LogicClientHome);
TITAN_PENDING_ENTRY(PlayerProfile);
TITAN_PENDING_ENTRY(Notification);

// ---- 22202 MapPreviewMessage: dataref ----
class MapPreviewMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22202; }
    const char* getMessageTypeName() const override { return "MapPreviewMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), mapRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapRef_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> mapRef_;
};

// ---- 20406 MatchMakingCancelledMessage: empty ----
class MatchMakingCancelledMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20406; }
    const char* getMessageTypeName() const override { return "MatchMakingCancelledMessage"; }
};

// ---- 20405 MatchMakingStatusMessage ----
class MatchMakingStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20405; }
    const char* getMessageTypeName() const override { return "MatchMakingStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : ints_) stream().writeInt(x);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : ints_) x = stream().readInt();
        flag_ = stream().readBoolean();
    }
    i32 ints_[5] = {};
    bool flag_ = false;
};

// ---- 24108 MatchmakeFailedMessage: int ----
class MatchmakeFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24108; }
    const char* getMessageTypeName() const override { return "MatchmakeFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(errorCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readInt();
    }
    i32 errorCode_ = 0;
};

// ---- 14103 MatchmakeRequestMessage ----
class MatchmakeRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14103; }
    const char* getMessageTypeName() const override { return "MatchmakeRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), eventRef_);
        for (i32 x : vints_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        eventRef_ = DataReference::decodeNullable(stream());
        for (i32& x : vints_) x = stream().readVInt();
    }
    std::optional<DataReference> eventRef_;
    i32 vints_[4] = {};
};

// ---- 24399 MyAllianceMessage ----
class MyAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24399; }
    const char* getMessageTypeName() const override { return "MyAllianceMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0;
    bool hasAlliance_ = false;
    std::optional<DataReference> allianceRef_;
    std::unique_ptr<AllianceHeaderEntry> header_;
};

// ---- 20801 NotificationMessage: factory type + payload ----
class NotificationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20801; }
    const char* getMessageTypeName() const override { return "NotificationMessage"; }
    void encode() override;
    void decode() override;
    i32 notifType_ = 0;
    std::unique_ptr<Notification> payload_;
};

// ---- 24104 OutOfSyncMessage: 3 vint ----
class OutOfSyncMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24104; }
    const char* getMessageTypeName() const override { return "OutOfSyncMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : v_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : v_) x = stream().readVInt();
    }
    i32 v_[3] = {};
};

// ---- 24101 OwnHomeDataMessage ----
class OwnHomeDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24101; }
    const char* getMessageTypeName() const override { return "OwnHomeDataMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicClientHome> home_;
    std::unique_ptr<LogicClientAvatar> avatar_;
    i32 f152_ = 0;
};

// ---- 20171 PersonalBreakStartedMessage: int ----
class PersonalBreakStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20171; }
    const char* getMessageTypeName() const override { return "PersonalBreakStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(f132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readInt();
    }
    i32 f132_ = 0;
};

// ---- 14177 PlayAgainMessage: bool ----
class PlayAgainMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14177; }
    const char* getMessageTypeName() const override { return "PlayAgainMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(f132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readBoolean();
    }
    bool f132_ = false;
};

// ---- 24777 PlayAgainStatusMessage ----
class PlayAgainStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24777; }
    const char* getMessageTypeName() const override { return "PlayAgainStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!status_) throw pending_reverse("PlayAgainStatusMessage needs PlayAgainStatus");
        status_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        status_ = std::make_unique<PlayAgainStatus>();
        status_->decode(stream());
    }
    std::unique_ptr<PlayAgainStatus> status_;
};

// ---- 23774 PlayerJWTokenMessage: string ----
class PlayerJWTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23774; }
    const char* getMessageTypeName() const override { return "PlayerJWTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        token_ = stream().readString();
    }
    std::optional<std::string> token_;
};

// ---- 22111 PlayerMapGreenlightedMessage ----
class PlayerMapGreenlightedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22111; }
    const char* getMessageTypeName() const override { return "PlayerMapGreenlightedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        stream().writeVInt(f140_);
        stream().writeVInt(f144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        f140_ = stream().readVInt();
        f144_ = stream().readVInt();
    }
    LogicLong mapId_;
    i32 f140_ = 0, f144_ = 0;
};

// ---- 22107 PlayerMapInfoUpdatedMessage ----
class PlayerMapInfoUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22107; }
    const char* getMessageTypeName() const override { return "PlayerMapInfoUpdatedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        for (i32 x : v_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        for (i32& x : v_) x = stream().readVInt();
    }
    LogicLong mapId_;
    i32 v_[5] = {};
};

// ---- 22102 PlayerMapsMessage: LogicPlayerMap array ----
// Binary treats count -1 (null) as empty on decode.
class PlayerMapsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22102; }
    const char* getMessageTypeName() const override { return "PlayerMapsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<LogicPlayerMap>> maps_;
    bool isNull_ = false;
};

// ---- 24113 PlayerProfileMessage ----
class PlayerProfileMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24113; }
    const char* getMessageTypeName() const override { return "PlayerProfileMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<PlayerProfile> profile_;
    bool hasAlliance_ = false;
    std::unique_ptr<AllianceHeaderEntry> alliance_;
    std::optional<DataReference> ref152_;
};

// ---- 14366 PlayerStatusMessage: vint ----
class PlayerStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14366; }
    const char* getMessageTypeName() const override { return "PlayerStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(status_);
    }
    void decode() override {
        PiranhaMessage::decode();
        status_ = stream().readVInt();
    }
    i32 status_ = 0;
};

// ---- 12105 PublishPlayerMapMessage: logiclong ----
class PublishPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12105; }
    const char* getMessageTypeName() const override { return "PublishPlayerMapMessage"; }
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

// ---- 22105 PublishPlayerMapResponseMessage ----
class PublishPlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22105; }
    const char* getMessageTypeName() const override { return "PublishPlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        mapId_.encode(stream());
        stream().writeVInt(f144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        f144_ = stream().readVInt();
    }
    i32 result_ = 0, f144_ = 0;
    LogicLong mapId_;
};

// ---- 22957 PvpMatchmakeNotificationMessage: vint ----
class PvpMatchmakeNotificationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22957; }
    const char* getMessageTypeName() const override { return "PvpMatchmakeNotificationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(f132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readVInt();
    }
    i32 f132_ = 0;
};

// ---- 22153 RankedMatchBanEndedMessage: LogicMapInt ----
class RankedMatchBanEndedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22153; }
    const char* getMessageTypeName() const override { return "RankedMatchBanEndedMessage"; }
    void encode() override;
    void decode() override;
    // (key, charRef) pairs
    std::vector<std::pair<i32, DataReference>> bans_;
};

// ---- 12152 RankedMatchBanHeroMessage ----
// Decode skips slot when at end (older clients).
class RankedMatchBanHeroMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12152; }
    const char* getMessageTypeName() const override { return "RankedMatchBanHeroMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), charRef_);
        stream().writeVInt(slot_);
    }
    void decode() override {
        PiranhaMessage::decode();
        charRef_ = DataReference::decodeNullable(stream());
        hasSlot_ = !stream().isAtEnd();
        if (hasSlot_) slot_ = stream().readVInt();
    }
    std::optional<DataReference> charRef_;
    i32 slot_ = 0;
    bool hasSlot_ = false;
};

// ---- 22152 RankedMatchBanHeroResponseMessage ----
class RankedMatchBanHeroResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22152; }
    const char* getMessageTypeName() const override { return "RankedMatchBanHeroResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(f132_);
        DataReference::encodeNullable(stream(), charRef_);
        stream().writeBoolean(f144_);
        stream().writeVInt(f148_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readVInt();
        charRef_ = DataReference::decodeNullable(stream());
        f144_ = stream().readBoolean();
        f148_ = stream().readVInt();
    }
    i32 f132_ = 0, f148_ = 0;
    std::optional<DataReference> charRef_;
    bool f144_ = false;
};

// ---- 22151 RankedMatchBanStartedMessage: 3 vint ----
class RankedMatchBanStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22151; }
    const char* getMessageTypeName() const override { return "RankedMatchBanStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : v_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : v_) x = stream().readVInt();
    }
    i32 v_[3] = {};
};

} // namespace titan
