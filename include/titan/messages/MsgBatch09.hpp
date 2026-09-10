#pragma once

// Message batch 09 — reversed from libg_decrypted.so (ARM64).
// Classes: SearchAlliances .. StartGame.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch02.hpp" // LogicGemOffer
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(RewardEntry);

// ---- 14324 SearchAlliancesMessage (verified by hand @0x2d52dc) ----
// Wire: string, int x4, bool, int, int (last two swapped vs layout).
class SearchAlliancesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14324; }
    const char* getMessageTypeName() const override { return "SearchAlliancesMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(text_ ? &*text_ : nullptr);
        for (i32 x : ints_) stream().writeInt(x);
        stream().writeBoolean(flag_);
        stream().writeInt(p168_);
        stream().writeInt(p160_);
    }
    void decode() override {
        PiranhaMessage::decode();
        text_ = stream().readString();
        for (i32& x : ints_) x = stream().readInt();
        flag_ = stream().readBoolean();
        p168_ = stream().readInt();
        p160_ = stream().readInt();
    }
    std::optional<std::string> text_;
    i32 ints_[4] = {};
    bool flag_ = false;
    i32 p168_ = 0, p160_ = 0;
};

// ---- 24123 SeasonRewardsMessage ----
// NOTE: same numeric id as DailyEventsMessage (24123); flagged re-verify.
class SeasonRewardsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24123; }
    const char* getMessageTypeName() const override { return "SeasonRewardsMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0;
    std::vector<std::unique_ptr<RewardEntry>> rewards_;
    bool hasOffer_ = false;
    std::unique_ptr<LogicGemOffer> offer_;
};

// ---- 14322 SendAllianceInvitationMessage: logiclong ----
class SendAllianceInvitationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14322; }
    const char* getMessageTypeName() const override { return "SendAllianceInvitationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
    }
    LogicLong avatarId_;
};

// ---- 14326 SendAllianceInvitationToFriendMessage ----
class SendAllianceInvitationToFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14326; }
    const char* getMessageTypeName() const override { return "SendAllianceInvitationToFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
        stream().writeString(s144_ ? &*s144_ : nullptr);
        stream().writeString(s152_ ? &*s152_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
        s144_ = stream().readString();
        s152_ = stream().readString();
    }
    LogicLong avatarId_;
    std::optional<std::string> s144_, s152_;
};

// ---- 14330 SendAllianceMailMessage ----
class SendAllianceMailMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14330; }
    const char* getMessageTypeName() const override { return "SendAllianceMailMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(mode_);
        stream().writeString(content_ ? &*content_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        mode_ = stream().readInt();
        content_ = stream().readString();
    }
    i32 mode_ = 0;
    std::optional<std::string> content_;
};

// ---- 14715 SendGlobalChatLineMessage: string ----
class SendGlobalChatLineMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14715; }
    const char* getMessageTypeName() const override { return "SendGlobalChatLineMessage"; }
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

// ---- 24115 ServerErrorMessage: int ----
class ServerErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24115; }
    const char* getMessageTypeName() const override { return "ServerErrorMessage"; }
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

// ---- 20100 ServerHelloMessage: bytes session key (decode caps 1000) ----
class ServerHelloMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20100; }
    const char* getMessageTypeName() const override { return "ServerHelloMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (sessionKey_) {
            const auto& p = *sessionKey_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        sessionKey_ = stream().readBytesNullable();
    }
    std::optional<std::vector<u8>> sessionKey_;
};

// ---- 14299 SetAllianceCountryMessage: dataref ----
class SetAllianceCountryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14299; }
    const char* getMessageTypeName() const override { return "SetAllianceCountryMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), country_);
    }
    void decode() override {
        PiranhaMessage::decode();
        country_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> country_;
};

// ---- 10576 SetBlockFriendRequestsMessage: bool ----
class SetBlockFriendRequestsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10576; }
    const char* getMessageTypeName() const override { return "SetBlockFriendRequestsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(blocked_);
    }
    void decode() override {
        PiranhaMessage::decode();
        blocked_ = stream().readBoolean();
    }
    bool blocked_ = false;
};

// ---- 12998 SetCountryMessage: dataref ----
class SetCountryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12998; }
    const char* getMessageTypeName() const override { return "SetCountryMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), country_);
    }
    void decode() override {
        PiranhaMessage::decode();
        country_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> country_;
};

// ---- 24178 SetCountryResponseMessage ----
class SetCountryResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24178; }
    const char* getMessageTypeName() const override { return "SetCountryResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        DataReference::encodeNullable(stream(), country_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        country_ = DataReference::decodeNullable(stream());
    }
    i32 result_ = 0;
    std::optional<DataReference> country_;
};

// ---- 10113 SetDeviceTokenMessage: bytes + int ----
class SetDeviceTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10113; }
    const char* getMessageTypeName() const override { return "SetDeviceTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (token_) {
            const auto& p = *token_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
        stream().writeInt(platform_);
    }
    void decode() override {
        PiranhaMessage::decode();
        token_ = stream().readBytesNullable();
        platform_ = stream().readInt();
    }
    std::optional<std::vector<u8>> token_;
    i32 platform_ = 0;
};

// ---- 20000 SetEncryptionMessage: bytes key ----
class SetEncryptionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20000; }
    const char* getMessageTypeName() const override { return "SetEncryptionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (key_) {
            const auto& p = *key_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        key_ = stream().readBytesNullable();
    }
    std::optional<std::vector<u8>> key_;
};

// ---- 14777 SetInvitesBlockedMessage: bool ----
class SetInvitesBlockedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14777; }
    const char* getMessageTypeName() const override { return "SetInvitesBlockedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(blocked_);
    }
    void decode() override {
        PiranhaMessage::decode();
        blocked_ = stream().readBoolean();
    }
    bool blocked_ = false;
};

// ---- 14867 SetRegionMessage: vint ----
class SetRegionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14867; }
    const char* getMessageTypeName() const override { return "SetRegionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(regionId_);
    }
    void decode() override {
        PiranhaMessage::decode();
        regionId_ = stream().readVInt();
    }
    i32 regionId_ = 0;
};

// ---- 24177 SetRegionResponseMessage: vint + vint ----
class SetRegionResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24177; }
    const char* getMessageTypeName() const override { return "SetRegionResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeVInt(region_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        region_ = stream().readVInt();
    }
    i32 result_ = 0, region_ = 0;
};

// ---- 18686 SetSupportedCreatorMessage: stringref ----
class SetSupportedCreatorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 18686; }
    const char* getMessageTypeName() const override { return "SetSupportedCreatorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(code_);
    }
    void decode() override {
        PiranhaMessage::decode();
        code_ = stream().readStringReference();
    }
    std::string code_;
};

// ---- 28686 SetSupportedCreatorResponseMessage ----
class SetSupportedCreatorResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28686; }
    const char* getMessageTypeName() const override { return "SetSupportedCreatorResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeStringReference(code_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        code_ = stream().readStringReference();
    }
    i32 result_ = 0;
    std::string code_;
};

// ---- 14778 SetTeamChatMutedMessage: bool ----
class SetTeamChatMutedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14778; }
    const char* getMessageTypeName() const override { return "SetTeamChatMutedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(muted_);
    }
    void decode() override {
        PiranhaMessage::decode();
        muted_ = stream().readBoolean();
    }
    bool muted_ = false;
};

// ---- 20161 ShutdownStartedMessage: int ----
class ShutdownStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20161; }
    const char* getMessageTypeName() const override { return "ShutdownStartedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(seconds_);
    }
    void decode() override {
        PiranhaMessage::decode();
        seconds_ = stream().readInt();
    }
    i32 seconds_ = 0;
};

// ---- 12111 SignoffPlayerMapMessage: 2 logiclong ----
class SignoffPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12111; }
    const char* getMessageTypeName() const override { return "SignoffPlayerMapMessage"; }
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

// ---- 14118 SinglePlayerMatchRequestMessage ----
class SinglePlayerMatchRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14118; }
    const char* getMessageTypeName() const override { return "SinglePlayerMatchRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), ref136_);
        DataReference::encodeNullable(stream(), ref144_);
        stream().writeVInt(p152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        ref136_ = DataReference::decodeNullable(stream());
        ref144_ = DataReference::decodeNullable(stream());
        p152_ = stream().readVInt();
    }
    std::optional<DataReference> ref136_, ref144_;
    i32 p152_ = 0;
};

// ---- 24105 SpectateFailedMessage: int ----
class SpectateFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24105; }
    const char* getMessageTypeName() const override { return "SpectateFailedMessage"; }
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

// ---- 14103 StartGameMessage: int ----
// NOTE: same numeric id as MatchmakeRequestMessage's family member 14103?
// (MatchmakeRequest is 14103 too — flagged re-verify.)
class StartGameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14103; }
    const char* getMessageTypeName() const override { return "StartGameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(p132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        p132_ = stream().readInt();
    }
    i32 p132_ = 0;
};

} // namespace titan
