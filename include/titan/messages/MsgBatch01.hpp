#pragma once

// Message batch 01 — reversed from libg_decrypted.so (ARM64).
// Classes: AntiAddictionDataUpdated .. AvatarNameChangeFailed.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(HeroDataEntry);
TITAN_PENDING_ENTRY(LogicCommand);

// ---- 20931 AntiAddictionDataUpdatedMessage: int + int ----
class AntiAddictionDataUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20931; }
    const char* getMessageTypeName() const override { return "AntiAddictionDataUpdatedMessage"; }
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

// ---- 24758 ApiTokenMessage: vint + string + vint ----
class ApiTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24758; }
    const char* getMessageTypeName() const override { return "ApiTokenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(fA_);
        stream().writeString(token_ ? &*token_ : nullptr);
        stream().writeVInt(fB_);
    }
    void decode() override {
        PiranhaMessage::decode();
        fA_ = stream().readVInt();
        token_ = stream().readString();
        fB_ = stream().readVInt();
    }
    i32 fA_ = 0, fB_ = 0;
    std::optional<std::string> token_;
};

// ---- 20151 AppleBillingProcessedByServerMessage ----
class AppleBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20151; }
    const char* getMessageTypeName() const override { return "AppleBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(f1_ ? &*f1_ : nullptr);
        stream().writeString(f2_ ? &*f2_ : nullptr);
        stream().writeInt(f3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readString();
        f2_ = stream().readString();
        f3_ = stream().readInt();
    }
    std::optional<std::string> f1_, f2_;
    i32 f3_ = 0;
};

// ---- 10150 AppleBillingRequestMessage: 4 strings + bytes ----
class AppleBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10150; }
    const char* getMessageTypeName() const override { return "AppleBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&s1_, &s2_, &s3_, &s4_})
            stream().writeString(*s ? &**s : nullptr);
        stream().writeBytes(payload_.empty() ? nullptr : payload_.data(),
                            static_cast<i32>(payload_.size()));
    }
    void decode() override {
        PiranhaMessage::decode();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
        s4_ = stream().readString();
        payload_ = stream().readBytes();
    }
    std::optional<std::string> s1_, s2_, s3_, s4_;
    std::vector<u8> payload_;
};

// ---- 16939 AskApiTokenMessage: empty ----
class AskApiTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16939; }
    const char* getMessageTypeName() const override { return "AskApiTokenMessage"; }
};

// ---- 10503 AskForAddableFriendsMessage ----
class AskForAddableFriendsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10503; }
    const char* getMessageTypeName() const override { return "AskForAddableFriendsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::string> idsA_, idsB_;
    bool nullA_ = false, nullB_ = false; // -> writeInt(-1)
};

// ---- 14302 AskForAllianceDataMessage ----
class AskForAllianceDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14302; }
    const char* getMessageTypeName() const override { return "AskForAllianceDataMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
        stream().writeBoolean(hasSecond_);
        if (hasSecond_) secondId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
        hasSecond_ = stream().readBoolean();
        if (hasSecond_) secondId_ = LogicLong::decode(stream());
    }
    LogicLong allianceId_, secondId_;
    bool hasSecond_ = false;
};

// ---- 14304 AskForAllianceStreamMessage: empty ----
class AskForAllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14304; }
    const char* getMessageTypeName() const override { return "AskForAllianceStreamMessage"; }
};

// ---- 14405 AskForAvatarStreamMessage: empty ----
class AskForAvatarStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14405; }
    const char* getMessageTypeName() const override { return "AskForAvatarStreamMessage"; }
};

// ---- 14110 AskForBattleEndMessage ----
class AskForBattleEndMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14110; }
    const char* getMessageTypeName() const override { return "AskForBattleEndMessage"; }
    void encode() override;
    void decode() override;
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
    std::optional<DataReference> location_;
    std::vector<std::unique_ptr<HeroDataEntry>> heroes_;
};

// ---- 14406 AskForBattleReplayStreamMessage: logiclong ----
class AskForBattleReplayStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14406; }
    const char* getMessageTypeName() const override { return "AskForBattleReplayStreamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        battleId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        battleId_ = LogicLong::decode(stream());
    }
    LogicLong battleId_;
};

// ---- 14303 AskForJoinableAlliancesListMessage: empty ----
class AskForJoinableAlliancesListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14303; }
    const char* getMessageTypeName() const override { return "AskForJoinableAlliancesListMessage"; }
};

// Helper for the AskForPlaying*Friends family (count + id list).
class PlayingFriendsQuery : public PiranhaMessage {
public:
    void encodeIds(bool useVInt, bool useRef);
    void decodeIds(bool useVInt, bool useRef);
    std::vector<std::string> ids_;
    bool null_ = false;
};

#define TITAN_PLAYING_FRIENDS(Name, Id, VInt, Ref)                             \
    class Name : public PlayingFriendsQuery {                                  \
    public:                                                                    \
        int getMessageType() const override { return Id; }                     \
        const char* getMessageTypeName() const override { return #Name; }      \
        void encode() override {                                               \
            PiranhaMessage::encode();                                          \
            encodeIds(VInt, Ref);                                              \
        }                                                                      \
        void decode() override {                                               \
            PiranhaMessage::decode();                                          \
            decodeIds(VInt, Ref);                                              \
        }                                                                      \
    }

TITAN_PLAYING_FRIENDS(AskForPlayingFacebookFriendsMessage, 10513, false, false);
TITAN_PLAYING_FRIENDS(AskForPlayingGamecenterFriendsMessage, 10512, false, false);
TITAN_PLAYING_FRIENDS(AskForPlayingKakaoFriendsMessage, 10514, false, false);
TITAN_PLAYING_FRIENDS(AskForPlayingLineFriendsMessage, 10516, false, false);
TITAN_PLAYING_FRIENDS(AskForPlayingSupercellFriendsMessage, 10517, false, false);
TITAN_PLAYING_FRIENDS(AskForPlayingTencentFriendsMessage, 10515, true, true);

#undef TITAN_PLAYING_FRIENDS

// ---- 10055 AskPlayerJWTokenMessage: empty ----
class AskPlayerJWTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10055; }
    const char* getMessageTypeName() const override { return "AskPlayerJWTokenMessage"; }
};

// ---- 10321 AttributionEventMessage: 9 stringrefs + bool ----
class AttributionEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10321; }
    const char* getMessageTypeName() const override { return "AttributionEventMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (const auto& p : params_) stream().writeStringReference(p);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (auto& p : params_) p = stream().readStringReference();
        flag_ = stream().readBoolean();
    }
    std::string params_[9];
    bool flag_ = false;
};

// ---- 30000 AttributionMessage ----
class AttributionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 30000; }
    const char* getMessageTypeName() const override { return "AttributionMessage"; }
    void encode() override;
    void decode() override;
    std::string head_[11];   // s_136..s_312
    i32 vintField_ = 0;      // field_612
    i64 longA_ = 0, longB_ = 0; // field_600/592 via writeLongLong
    bool flags_[4] = {};     // 616, 608, 617, 328
    std::string tail_[16];   // s_336..s_576
    std::string last_;       // s_216
};

// ---- 10112 AuthenticationCheckMessage: empty ----
class AuthenticationCheckMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10112; }
    const char* getMessageTypeName() const override { return "AuthenticationCheckMessage"; }
};

// ---- 20104 AuthenticationResponseMessage: bool ----
class AuthenticationResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20104; }
    const char* getMessageTypeName() const override { return "AuthenticationResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(success_);
    }
    void decode() override {
        PiranhaMessage::decode();
        success_ = stream().readBoolean();
    }
    bool success_ = false;
};

// ---- 24111 AvailableServerCommandMessage: LogicCommand ----
class AvailableServerCommandMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24111; }
    const char* getMessageTypeName() const override { return "AvailableServerCommandMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCommand> command_;
};

// ---- 20205 AvatarNameChangeFailedMessage: int ----
class AvatarNameChangeFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20205; }
    const char* getMessageTypeName() const override { return "AvatarNameChangeFailedMessage"; }
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

} // namespace titan
