#pragma once

// Message batch 06 — reversed from libg_decrypted.so (ARM64).
// Classes: KunlunBillingRequest .. LookForGameRoomRequest.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/game/LogicCompressedString.hpp" // real LogicCompressedString
#include "titan/messages/MsgBatch04.hpp" // LogicClientAvatar
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

TITAN_PENDING_ENTRY(LatencyData);
TITAN_PENDING_ENTRY(RankingEntry);
TITAN_PENDING_ENTRY(LobbyInfoEntry);

// ---- 10159 KunlunBillingRequestMessage: 4 stringrefs ----
class KunlunBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10159; }
    const char* getMessageTypeName() const override { return "KunlunBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (const auto& s : fields_) stream().writeStringReference(s);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (auto& s : fields_) s = stream().readStringReference();
    }
    std::string fields_[4];
};

// ---- 19001 LatencyTestResultMessage ----
class LatencyTestResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19001; }
    const char* getMessageTypeName() const override { return "LatencyTestResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : vints_) stream().writeVInt(x);
        stream().writeBoolean(flag_);
        stream().writeString(tail_ ? &*tail_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : vints_) x = stream().readVInt();
        flag_ = stream().readBoolean();
        tail_ = stream().readString();
    }
    i32 vints_[7] = {};
    bool flag_ = false;
    std::optional<std::string> tail_;
};

// ---- 29003 LatencyTestStatusMessage ----
class LatencyTestStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29003; }
    const char* getMessageTypeName() const override { return "LatencyTestStatusMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<LatencyData>> entries_;
};

// ---- 24403 LeaderboardMessage ----
class LeaderboardMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24403; }
    const char* getMessageTypeName() const override { return "LeaderboardMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0, v136_ = 0;
    std::optional<DataReference> ref_;
    std::optional<std::string> name_;
    std::vector<std::unique_ptr<RankingEntry>> rankings_;
    i32 tail_[4] = {}; // v168..v180
    std::optional<std::string> tailStr_;
};

// ---- 14308 LeaveAllianceMessage: empty ----
class LeaveAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14308; }
    const char* getMessageTypeName() const override { return "LeaveAllianceMessage"; }
};

// ---- 24205 LineAccountAlreadyBoundMessage ----
class LineAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24205; }
    const char* getMessageTypeName() const override { return "LineAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> id_, name_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

// ---- 24206 LineAccountBoundMessage: int ----
class LineAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24206; }
    const char* getMessageTypeName() const override { return "LineAccountBoundMessage"; }
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

// ---- 24216 LineAccountUnboundMessage: empty ----
class LineAccountUnboundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24216; }
    const char* getMessageTypeName() const override { return "LineAccountUnboundMessage"; }
};

// ---- 14700 ListBrawlTvChannelsMessage: vint + bool ----
class ListBrawlTvChannelsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14700; }
    const char* getMessageTypeName() const override { return "ListBrawlTvChannelsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
        flag_ = stream().readBoolean();
    }
    i32 v_ = 0;
    bool flag_ = false;
};

// ---- 23457 LobbyInfoMessage ----
class LobbyInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23457; }
    const char* getMessageTypeName() const override { return "LobbyInfoMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0;
    std::optional<std::string> name_;
    std::vector<std::unique_ptr<LobbyInfoEntry>> entries_;
};

// ---- 17000 LogicAccountTransferCodeRequestMessage: empty ----
class LogicAccountTransferCodeRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17000; }
    const char* getMessageTypeName() const override { return "LogicAccountTransferCodeRequestMessage"; }
};

// ---- 27002 LogicAccountTransferCodeResponseMessage ----
class LogicAccountTransferCodeResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 27002; }
    const char* getMessageTypeName() const override { return "LogicAccountTransferCodeResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(code_);
        stream().writeInt(f156_);
        stream().writeInt(f152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        code_ = stream().readStringReference();
        f156_ = stream().readInt();
        f152_ = stream().readInt();
    }
    std::string code_;
    i32 f156_ = 0, f152_ = 0;
};

// ---- 26004 LogicDeviceLinkCodeDeactivatedMessage: empty ----
class LogicDeviceLinkCodeDeactivatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26004; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkCodeDeactivatedMessage"; }
};

// ---- 16000 LogicDeviceLinkCodeRequestMessage: empty ----
class LogicDeviceLinkCodeRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16000; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkCodeRequestMessage"; }
};

// ---- 26002 LogicDeviceLinkCodeResponseMessage ----
class LogicDeviceLinkCodeResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26002; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkCodeResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(code_);
        stream().writeInt(f156_);
        stream().writeInt(f152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        code_ = stream().readStringReference();
        f156_ = stream().readInt();
        f152_ = stream().readInt();
    }
    std::string code_;
    i32 f156_ = 0, f152_ = 0;
};

// ---- 16003 LogicDeviceLinkConfirmYesMessage ----
class LogicDeviceLinkConfirmYesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16003; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkConfirmYesMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
        stream().writeStringReference(f160_);
        stream().writeStringReference(f176_);
        id_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
        f160_ = stream().readStringReference();
        f176_ = stream().readStringReference();
        id_ = LogicLong::decode(stream());
    }
    std::string f136_, f160_, f176_;
    LogicLong id_;
};

// ---- 26007 LogicDeviceLinkDoneMessage ----
class LogicDeviceLinkDoneMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26007; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkDoneMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f144_);
        id_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        f144_ = stream().readStringReference();
        id_ = LogicLong::decode(stream());
    }
    std::string f144_;
    LogicLong id_;
};

// ---- 16002 LogicDeviceLinkEnterCodeMessage ----
class LogicDeviceLinkEnterCodeMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16002; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkEnterCodeMessage"; }
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

// ---- 26008 LogicDeviceLinkErrorMessage ----
class LogicDeviceLinkErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26008; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(f132_);
        stream().writeInt(f136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readBoolean();
        f136_ = stream().readInt();
    }
    bool f132_ = false;
    i32 f136_ = 0;
};

// ---- 16001 LogicDeviceLinkMenuClosedMessage: empty ----
class LogicDeviceLinkMenuClosedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16001; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkMenuClosedMessage"; }
};

// ---- 26003 LogicDeviceLinkNewDeviceLinkedMessage ----
class LogicDeviceLinkNewDeviceLinkedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26003; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkNewDeviceLinkedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
    }
    std::string f136_;
};

// ---- 26005 LogicDeviceLinkResponseMessage ----
class LogicDeviceLinkResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26005; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
        stream().writeStringReference(f168_);
        stream().writeInt(f156_);
        stream().writeInt(f160_);
        stream().writeInt(f152_);
        stream().writeStringReference(f192_);
        id_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
        f168_ = stream().readStringReference();
        f156_ = stream().readInt();
        f160_ = stream().readInt();
        f152_ = stream().readInt();
        f192_ = stream().readStringReference();
        id_ = LogicLong::decode(stream());
    }
    std::string f136_, f168_, f192_;
    i32 f156_ = 0, f160_ = 0, f152_ = 0;
    LogicLong id_;
};

// ---- 20104 LoginOkMessage ----
// Encode writes everything unconditionally; decode guards the f232.. tail
// with isAtEnd() (shorter variants from older servers).
class LoginOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20104; }
    const char* getMessageTypeName() const override { return "LoginOkMessage"; }
    void encode() override;
    void decode() override;
    LogicLong accountId_, homeId_;
    std::optional<std::string> passToken_, f160_, f168_;
    i32 f192_ = 0, f196_ = 0, f200_ = 0;
    std::optional<std::string> f208_;
    i32 f216_ = 0, f220_ = 0, f224_ = 0;
    std::optional<std::string> f184_;
    // isAtEnd-guarded tail:
    std::optional<std::string> f232_, f240_;
    i32 f248_ = 0;
    bool hasTail1_ = false;
    std::optional<std::string> f176_, f256_, f264_;
    i32 f312_ = 0;
    bool hasTail2_ = false;
    std::optional<std::string> f272_;
    std::vector<std::string> list296_, list304_; // capped at 100
    bool hasLists_ = false;
    i32 f332_ = 0;
    bool hasF332_ = false;
    std::unique_ptr<LogicCompressedString> compressed_;
    bool hasCompressed_ = false;
    bool f328_ = false, f329not_ = false;
    bool hasFlags_ = false;
    std::optional<std::string> f280_, f336_, f288_, f344_, f352_;
    bool f360_ = false;
    bool hasTail3_ = false;
};

// ---- 10102 LoginUsingSessionMessage ----
class LoginUsingSessionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10102; }
    const char* getMessageTypeName() const override { return "LoginUsingSessionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(f132_);
        stream().writeInt(f136_);
        for (auto* s : {&f144_, &f152_, &f160_, &f168_, &f176_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readInt();
        f136_ = stream().readInt();
        f144_ = stream().readString();
        f152_ = stream().readString();
        f160_ = stream().readString();
        f168_ = stream().readString();
        f176_ = stream().readString();
    }
    i32 f132_ = 0, f136_ = 0;
    std::optional<std::string> f144_, f152_, f160_, f168_, f176_;
};

// ---- 14199 LookForGameRoomRequestMessage ----
class LookForGameRoomRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14199; }
    const char* getMessageTypeName() const override { return "LookForGameRoomRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(f132_);
        stream().writeVInt(f136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readVInt();
        f136_ = stream().readVInt();
    }
    i32 f132_ = 0, f136_ = 0;
};

} // namespace titan
