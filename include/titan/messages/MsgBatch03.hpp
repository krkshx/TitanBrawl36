#pragma once

// Message batch 03 — reversed from libg_decrypted.so (ARM64).
// Classes: ChangeAllianceMemberRole .. DebugCommand.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch00.hpp" // AllianceFullEntry
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(LogicPlayerMap);
TITAN_PENDING_ENTRY(EventData);

// ---- 14306 ChangeAllianceMemberRoleMessage ----
class ChangeAllianceMemberRoleMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14306; }
    const char* getMessageTypeName() const override { return "ChangeAllianceMemberRoleMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
        stream().writeVInt(role_);
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
        role_ = stream().readVInt();
    }
    LogicLong avatarId_;
    i32 role_ = 0;
};

// Shared shape: ChangeAllianceSettings (14316) and CreateAlliance (14301)
// differ only in the leading strings.
class AllianceSettingsBase : public PiranhaMessage {
public:
    void encodeSettings();
    void decodeSettings();
    std::optional<std::string> name_, description_;
    std::optional<DataReference> badge_, region_;
    i32 allianceType_ = 0, requiredTrophies_ = 0;
    bool familyFriendly_ = false;
};

// ---- 14316 ChangeAllianceSettingsMessage ----
class ChangeAllianceSettingsMessage : public AllianceSettingsBase {
public:
    int getMessageType() const override { return 14316; }
    const char* getMessageTypeName() const override { return "ChangeAllianceSettingsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeSettings();
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeSettings();
    }
};

// ---- 24313 ChangeAllianceSettingsOkMessage ----
class ChangeAllianceSettingsOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24313; }
    const char* getMessageTypeName() const override { return "ChangeAllianceSettingsOkMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!entry_) throw pending_reverse("ChangeAllianceSettingsOkMessage needs AllianceFullEntry");
        entry_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        entry_ = std::make_unique<AllianceFullEntry>();
        entry_->decode(stream());
    }
    std::unique_ptr<AllianceFullEntry> entry_;
};

// ---- 10212 ChangeAvatarNameMessage: string + bool ----
class ChangeAvatarNameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10212; }
    const char* getMessageTypeName() const override { return "ChangeAvatarNameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(newName_ ? &*newName_ : nullptr);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        newName_ = stream().readString();
        flag_ = stream().readBoolean();
    }
    std::optional<std::string> newName_;
    bool flag_ = false;
};

// ---- 12106 ChangePlayerMapNameMessage ----
class ChangePlayerMapNameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12106; }
    const char* getMessageTypeName() const override { return "ChangePlayerMapNameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        stream().writeString(newName_ ? &*newName_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        newName_ = stream().readString();
    }
    LogicLong mapId_;
    std::optional<std::string> newName_;
};

// ---- 22106 ChangePlayerMapNameResponseMessage ----
class ChangePlayerMapNameResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22106; }
    const char* getMessageTypeName() const override { return "ChangePlayerMapNameResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(resultCode_);
        mapId_.encode(stream());
        stream().writeString(name_ ? &*name_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        name_ = stream().readString();
    }
    i32 resultCode_ = 0;
    LogicLong mapId_;
    std::optional<std::string> name_;
};

// ---- 20118 ChatAccountBanStatusMessage: int, no base encode in binary ----
// (Base encode is a no-op here, so wire bytes are identical either way.)
class ChatAccountBanStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20118; }
    const char* getMessageTypeName() const override { return "ChatAccountBanStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(banStatus_);
    }
    void decode() override {
        PiranhaMessage::decode();
        banStatus_ = stream().readInt();
    }
    i32 banStatus_ = 0;
};

// ---- 14315 ChatToAllianceStreamMessage: string ----
class ChatToAllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14315; }
    const char* getMessageTypeName() const override { return "ChatToAllianceStreamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(chat_ ? &*chat_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        chat_ = stream().readString();
    }
    std::optional<std::string> chat_;
};

// ---- 14166 ChronosEventSeenMessage: vint ----
class ChronosEventSeenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14166; }
    const char* getMessageTypeName() const override { return "ChronosEventSeenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(eventId_);
    }
    void decode() override {
        PiranhaMessage::decode();
        eventId_ = stream().readVInt();
    }
    i32 eventId_ = 0;
};

// ---- 10107 ClientCapabilitiesMessage: vint ----
class ClientCapabilitiesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10107; }
    const char* getMessageTypeName() const override { return "ClientCapabilitiesMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(flags_);
    }
    void decode() override {
        PiranhaMessage::decode();
        flags_ = stream().readVInt();
    }
    i32 flags_ = 0;
};

// ---- 10099 ClientCryptoErrorMessage: int ----
class ClientCryptoErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10099; }
    const char* getMessageTypeName() const override { return "ClientCryptoErrorMessage"; }
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

// ---- 10100 ClientHelloMessage ----
class ClientHelloMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10100; }
    const char* getMessageTypeName() const override { return "ClientHelloMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(protocolVersion_);
        stream().writeInt(keyVersion_);
        stream().writeInt(majorVersion_);
        stream().writeInt(minorVersion_);
        stream().writeInt(buildVersion_);
        stream().writeStringReference(fingerprintSha_);
        stream().writeInt(appStore_);
        stream().writeInt(deviceType_);
    }
    void decode() override {
        PiranhaMessage::decode();
        protocolVersion_ = stream().readInt();
        keyVersion_ = stream().readInt();
        majorVersion_ = stream().readInt();
        minorVersion_ = stream().readInt();
        buildVersion_ = stream().readInt();
        fingerprintSha_ = stream().readStringReference();
        appStore_ = stream().readInt();
        deviceType_ = stream().readInt();
    }
    i32 protocolVersion_ = 0, keyVersion_ = 0;
    i32 majorVersion_ = 0, minorVersion_ = 0, buildVersion_ = 0;
    std::string fingerprintSha_;
    i32 appStore_ = 0, deviceType_ = 0;
};

// ---- 10177 ClientInfoMessage: string ----
class ClientInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10177; }
    const char* getMessageTypeName() const override { return "ClientInfoMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(info_ ? &*info_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        info_ = stream().readString();
    }
    std::optional<std::string> info_;
};

// ---- 10555 ClientInputMessage: BitStream-packed, pending BitStream reverse ----
// Layout per binary: 5 clamped ints (16383/1023/8191/1023/1023) then a
// count-prefixed array of BitStream-encoded ClientInput entries.
class ClientInputMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10555; }
    const char* getMessageTypeName() const override { return "ClientInputMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        throw pending_reverse("ClientInputMessage needs BitStream");
    }
    void decode() override {
        PiranhaMessage::decode();
        throw pending_reverse("ClientInputMessage needs BitStream");
    }
};

// ---- 20101 CreateAccountFailedMessage: int ----
// NOTE: IDA shows the same type id as CreateAccountOkMessage; flagged
// for re-verify against getMessageType of both classes.
class CreateAccountFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20101; }
    const char* getMessageTypeName() const override { return "CreateAccountFailedMessage"; }
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

// ---- 10103 CreateAccountMessage: 5 strings ----
class CreateAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10103; }
    const char* getMessageTypeName() const override { return "CreateAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&s1_, &s2_, &s3_, &s4_, &s5_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
        s4_ = stream().readString();
        s5_ = stream().readString();
    }
    std::optional<std::string> s1_, s2_, s3_, s4_, s5_;
};

// ---- 20101 CreateAccountOkMessage ----
class CreateAccountOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20101; }
    const char* getMessageTypeName() const override { return "CreateAccountOkMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(accountIdHigh_);
        stream().writeInt(accountIdLow_);
        stream().writeString(passToken_ ? &*passToken_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountIdHigh_ = stream().readInt();
        accountIdLow_ = stream().readInt();
        passToken_ = stream().readString();
    }
    i32 accountIdHigh_ = 0, accountIdLow_ = 0;
    std::optional<std::string> passToken_;
};

// ---- 14301 CreateAllianceMessage: same shape as settings ----
class CreateAllianceMessage : public AllianceSettingsBase {
public:
    int getMessageType() const override { return 14301; }
    const char* getMessageTypeName() const override { return "CreateAllianceMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeSettings();
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeSettings();
    }
};

// ---- 20402 CreateGameFailedMessage: int ----
class CreateGameFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20402; }
    const char* getMessageTypeName() const override { return "CreateGameFailedMessage"; }
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

// ---- 10401 CreateGameMessage: int + int ----
class CreateGameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10401; }
    const char* getMessageTypeName() const override { return "CreateGameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(f1_);
        stream().writeInt(f2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readInt();
        f2_ = stream().readInt();
    }
    i32 f1_ = 0, f2_ = 0;
};

// ---- 12100 CreatePlayerMapMessage ----
class CreatePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12100; }
    const char* getMessageTypeName() const override { return "CreatePlayerMapMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(mapName_ ? &*mapName_ : nullptr);
        stream().writeVInt(v_);
        DataReference::encodeNullable(stream(), gameMode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapName_ = stream().readString();
        v_ = stream().readVInt();
        gameMode_ = DataReference::decodeNullable(stream());
    }
    std::optional<std::string> mapName_;
    i32 v_ = 0;
    std::optional<DataReference> gameMode_;
};

// ---- 22100 CreatePlayerMapResponseMessage ----
class CreatePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22100; }
    const char* getMessageTypeName() const override { return "CreatePlayerMapResponseMessage"; }
    void encode() override;
    void decode() override;
    i32 resultCode_ = 0;
    bool hasMap_ = false;
    std::unique_ptr<LogicPlayerMap> map_;
};

// ---- 29997 CryptoErrorMessage: vint ----
class CryptoErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29997; }
    const char* getMessageTypeName() const override { return "CryptoErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(errorCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readVInt();
    }
    i32 errorCode_ = 0;
};

// ---- 24123 DailyEventsMessage ----
class DailyEventsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24123; }
    const char* getMessageTypeName() const override { return "DailyEventsMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0, f136_ = 0;
    std::vector<std::unique_ptr<EventData>> events_;
};

// ---- 14178 DebugCommandMessage: int + int ----
class DebugCommandMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14178; }
    const char* getMessageTypeName() const override { return "DebugCommandMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(commandId_);
        stream().writeInt(param_);
    }
    void decode() override {
        PiranhaMessage::decode();
        commandId_ = stream().readInt();
        param_ = stream().readInt();
    }
    i32 commandId_ = 0, param_ = 0;
};

} // namespace titan
