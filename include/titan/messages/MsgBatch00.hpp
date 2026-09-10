#pragma once

// Message batch 00 — reversed from libg_decrypted.so (ARM64).
// Classes: AcceptFriendFailed .. AnalyticEvent (see data/message_batches.json).
// Scalar fields are bit-exact; nested entries not yet reversed are held as
// Streamable subclasses whose encode/decode throw pending_reverse until
// their own wave lands. Empty nested arrays (count 0) already round-trip.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(AddableFriendEntry);
TITAN_PENDING_ENTRY(AllianceFullEntry);
TITAN_PENDING_ENTRY(AllianceHeaderEntry);
TITAN_PENDING_ENTRY(AllianceMemberEntry);
TITAN_PENDING_ENTRY(AllianceTeamEntry);
TITAN_PENDING_ENTRY(AllianceWarNode);
TITAN_PENDING_ENTRY(AllianceWarFaction);
TITAN_PENDING_ENTRY(AnalyticEvent);
TITAN_PENDING_ENTRY(StatusChangeEntry);
TITAN_PENDING_ENTRY(StreamEntry);

// FriendAvatarBaseMessage::encode @0x672c90: two writeInt (idHigh, idLow).
struct FriendAvatarBase {
    i32 idHigh = 0;
    i32 idLow = 0;
    void encode(ByteStream& s) const {
        s.writeInt(idHigh);
        s.writeInt(idLow);
    }
    void decode(ByteStream& s) {
        idHigh = s.readInt();
        idLow = s.readInt();
    }
};

// ---- 20501 AcceptFriendFailedMessage: base + int reason ----
class AcceptFriendFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20501; }
    const char* getMessageTypeName() const override { return "AcceptFriendFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        reason_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 reason_ = 0;
};

// ---- 13922 AcceptTokenFriendMessage: base + string token ----
class AcceptTokenFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 13922; }
    const char* getMessageTypeName() const override { return "AcceptTokenFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        token_ = stream().readString();
    }
    FriendAvatarBase base_;
    std::optional<std::string> token_;
};

// ---- 10111 AccountIdentifiersMessage: string identifier ----
class AccountIdentifiersMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10111; }
    const char* getMessageTypeName() const override { return "AccountIdentifiersMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(id_ ? &*id_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        id_ = stream().readString();
    }
    std::optional<std::string> id_;
};

// ---- 10118 AccountSwitchedMessage ----
class AccountSwitchedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10118; }
    const char* getMessageTypeName() const override { return "AccountSwitchedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        accountId_.encode(stream());
        stream().writeBoolean(flag1_);
        stream().writeBoolean(flag2_);
        stream().writeString(env_ ? &*env_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountId_ = LogicLong::decode(stream());
        flag1_ = stream().readBoolean();
        flag2_ = stream().readBoolean();
        env_ = stream().readString();
    }
    LogicLong accountId_;
    bool flag1_ = false, flag2_ = false;
    std::optional<std::string> env_;
};

// ---- 40000 AdUpdateConversionValueMessage: int ----
class AdUpdateConversionValueMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 40000; }
    const char* getMessageTypeName() const override { return "AdUpdateConversionValueMessage"; }
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

// ---- 10509 AddFriendByAvatarNameAndCodeMessage ----
class AddFriendByAvatarNameAndCodeMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10509; }
    const char* getMessageTypeName() const override { return "AddFriendByAvatarNameAndCodeMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(name_ ? &*name_ : nullptr);
        stream().writeString(code_ ? &*code_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        name_ = stream().readString();
        code_ = stream().readString();
    }
    std::optional<std::string> name_, code_;
};

// ---- 10507 AddFriendByEmailMessage ----
class AddFriendByEmailMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10507; }
    const char* getMessageTypeName() const override { return "AddFriendByEmailMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(email_ ? &*email_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        email_ = stream().readString();
    }
    std::optional<std::string> email_;
};

// ---- 20112 AddFriendFailedMessage: int reason ----
class AddFriendFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20112; }
    const char* getMessageTypeName() const override { return "AddFriendFailedMessage"; }
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

// ---- 10502 AddFriendMessage: base + idHigh + idLow ----
class AddFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10502; }
    const char* getMessageTypeName() const override { return "AddFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(avatarIdHigh_);
        stream().writeInt(avatarIdLow_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        avatarIdHigh_ = stream().readInt();
        avatarIdLow_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 avatarIdHigh_ = 0, avatarIdLow_ = 0;
};

// ---- 20107 AddableFriendsMessage: int count + entries ----
class AddableFriendsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20107; }
    const char* getMessageTypeName() const override { return "AddableFriendsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<AddableFriendEntry>> entries_;
};

// ---- 17338 AdjustPackageMessage: int + bytes ----
class AdjustPackageMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17338; }
    const char* getMessageTypeName() const override { return "AdjustPackageMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(packageType_);
        stream().writeBytes(payload_.empty() ? nullptr : payload_.data(),
                            static_cast<i32>(payload_.size()));
    }
    void decode() override {
        PiranhaMessage::decode();
        packageType_ = stream().readInt();
        payload_ = stream().readBytes();
    }
    i32 packageType_ = 0;
    std::vector<u8> payload_;
};

// ---- 24301 AllianceDataMessage: bool + full entry ----
class AllianceDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24301; }
    const char* getMessageTypeName() const override { return "AllianceDataMessage"; }
    void encode() override;
    void decode() override;
    bool isMyAlliance_ = false;
    std::unique_ptr<AllianceFullEntry> entry_;
};

// ---- 24321 AllianceInvitationSendFailedMessage: int ----
class AllianceInvitationSendFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24321; }
    const char* getMessageTypeName() const override { return "AllianceInvitationSendFailedMessage"; }
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

// ---- 24310 AllianceListMessage: string + vint count + headers ----
class AllianceListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24310; }
    const char* getMessageTypeName() const override { return "AllianceListMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> searchQuery_;
    std::vector<std::unique_ptr<AllianceHeaderEntry>> headers_;
};

// ---- 24308 AllianceMemberMessage: logiclong + entry ----
class AllianceMemberMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24308; }
    const char* getMessageTypeName() const override { return "AllianceMemberMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    std::unique_ptr<AllianceMemberEntry> entry_;
};

// ---- 24309 AllianceMemberRemovedMessage: two logiclong ----
class AllianceMemberRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24309; }
    const char* getMessageTypeName() const override { return "AllianceMemberRemovedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
        allianceId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
        allianceId_ = LogicLong::decode(stream());
    }
    LogicLong avatarId_, allianceId_;
};

// ---- 20207 AllianceOnlineStatusUpdatedMessage ----
class AllianceOnlineStatusUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20207; }
    const char* getMessageTypeName() const override { return "AllianceOnlineStatusUpdatedMessage"; }
    void encode() override;
    void decode() override;
    i32 statusType_ = 0;
    std::vector<std::unique_ptr<StatusChangeEntry>> changes_;
};

// ---- 24333 AllianceResponseMessage: vint + vint ----
class AllianceResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24333; }
    const char* getMessageTypeName() const override { return "AllianceResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(responseType_);
        stream().writeVInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        responseType_ = stream().readVInt();
        reason_ = stream().readVInt();
    }
    i32 responseType_ = 0, reason_ = 0;
};

// ---- 24312 AllianceStreamEntryMessage: vint type + entry ----
class AllianceStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24312; }
    const char* getMessageTypeName() const override { return "AllianceStreamEntryMessage"; }
    void encode() override;
    void decode() override;
    i32 entryType_ = 0;
    std::unique_ptr<StreamEntry> entry_;
};

// ---- 24318 AllianceStreamEntryRemovedMessage: logiclong ----
class AllianceStreamEntryRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24318; }
    const char* getMessageTypeName() const override { return "AllianceStreamEntryRemovedMessage"; }
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

// ---- 24311 AllianceStreamMessage: count + typed entries ----
class AllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24311; }
    const char* getMessageTypeName() const override { return "AllianceStreamMessage"; }
    void encode() override;
    void decode() override;
    // (entryType, entry) pairs; empty vector encodes count -1? No:
    // count -1 means null; we always write the real count.
    std::vector<std::pair<i32, std::unique_ptr<StreamEntry>>> entries_;
    bool isNull_ = false; // -> writeVInt(-1)
};

// ---- 24365 AllianceTeamRemovedMessage: logiclong ----
class AllianceTeamRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24365; }
    const char* getMessageTypeName() const override { return "AllianceTeamRemovedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        teamId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        teamId_ = LogicLong::decode(stream());
    }
    LogicLong teamId_;
};

// ---- 24364 AllianceTeamsMessage: bool + team entries ----
class AllianceTeamsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24364; }
    const char* getMessageTypeName() const override { return "AllianceTeamsMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    std::vector<std::unique_ptr<AllianceTeamEntry>> teams_;
};

// ---- 24776 AllianceWarMessage ----
class AllianceWarMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24776; }
    const char* getMessageTypeName() const override { return "AllianceWarMessage"; }
    void encode() override;
    void decode() override;
    LogicLong allianceId_;
    i32 field144_ = 0;
    std::vector<std::unique_ptr<AllianceWarNode>> nodes_;
    std::vector<std::unique_ptr<AllianceWarFaction>> factions_;
};

// ---- 10110 AnalyticEventMessage: single nested event ----
class AnalyticEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10110; }
    const char* getMessageTypeName() const override { return "AnalyticEventMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<AnalyticEvent> event_;
};

} // namespace titan
