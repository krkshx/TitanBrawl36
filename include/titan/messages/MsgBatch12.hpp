#pragma once

// Message batch 12 — reversed from libg_decrypted.so (ARM64).
// Classes: TencentBillingRequest .. YoozooOrderAvailable.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch06.hpp" // LogicCompressedString
#include "titan/messages/MsgBatch11.hpp" // LogicTencentAntiAddictionInstruction
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

// ---- 10152 TencentBillingRequestMessage ----
// Wire order: +136, +152, +184, +168 (f3/f4 swapped vs layout).
class TencentBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10152; }
    const char* getMessageTypeName() const override { return "TencentBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f1_);
        stream().writeStringReference(f2_);
        stream().writeStringReference(f4_);
        stream().writeStringReference(f3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readStringReference();
        f2_ = stream().readStringReference();
        f4_ = stream().readStringReference();
        f3_ = stream().readStringReference();
    }
    std::string f1_, f2_, f3_, f4_;
};

// ---- 14268 TencentCheckCanPayMessage: vint ----
class TencentCheckCanPayMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14268; }
    const char* getMessageTypeName() const override { return "TencentCheckCanPayMessage"; }
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

// ---- 24223 TencentCheckCanPayResponseMessage ----
class TencentCheckCanPayResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24223; }
    const char* getMessageTypeName() const override { return "TencentCheckCanPayResponseMessage"; }
    void encode() override;
    void decode() override;
    std::string data_;
    std::vector<std::unique_ptr<LogicTencentAntiAddictionInstruction>> instructions_;
    bool isNull_ = false;
};

// ---- 25892 TitanDisconnectedMessage: int ----
// NOTE: same numeric id as DisconnectedMessage (25892, vint there vs int
// here); flagged re-verify.
class TitanDisconnectedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 25892; }
    const char* getMessageTypeName() const override { return "TitanDisconnectedMessage"; }
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

// ---- 19003 TriggerStartLatencyTestMessage: 3 vint ----
class TriggerStartLatencyTestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19003; }
    const char* getMessageTypeName() const override { return "TriggerStartLatencyTestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
        stream().writeVInt(v3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
        v3_ = stream().readVInt();
    }
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
};

// ---- 14701 TuneBrawlTvChannelMessage: vint + vint ----
class TuneBrawlTvChannelMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14701; }
    const char* getMessageTypeName() const override { return "TuneBrawlTvChannelMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

// ---- 20101 UdpBigMessageFragmentMessage ----
// NOTE: same numeric id as CreateAccount* (20101); flagged re-verify
// (UDP sub-protocol reuses the id space).
// Wire: byte frag, vint msgId, byte index, byte count, vint len + RAW bytes.
class UdpBigMessageFragmentMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20101; }
    const char* getMessageTypeName() const override { return "UdpBigMessageFragmentMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeByte(fragId_);
        stream().writeVInt(msgId_);
        stream().writeByte(index_);
        stream().writeByte(count_);
        stream().writeVInt(static_cast<i32>(payload_.size()));
        if (!payload_.empty()) stream().writeRawBytes(payload_.data(), static_cast<i32>(payload_.size()));
    }
    void decode() override {
        PiranhaMessage::decode();
        fragId_ = stream().readByte();
        msgId_ = stream().readVInt();
        index_ = stream().readByte();
        count_ = stream().readByte();
        const i32 n = stream().readVInt();
        payload_ = (n > 900000) ? std::vector<u8>{} : stream().readRawBytes(n);
    }
    i8 fragId_ = 0, index_ = 0, count_ = 0;
    i32 msgId_ = 0;
    std::vector<u8> payload_;
};

// ---- 10109 UdpCheckConnectionMessage: empty ----
class UdpCheckConnectionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10109; }
    const char* getMessageTypeName() const override { return "UdpCheckConnectionMessage"; }
};

// ---- 24112 UdpConnectionInfoMessage ----
class UdpConnectionInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24112; }
    const char* getMessageTypeName() const override { return "UdpConnectionInfoMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeString(addr_ ? &*addr_ : nullptr);
        if (key1_) {
            const auto& p = *key1_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
        if (key2_) {
            const auto& p = *key2_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        addr_ = stream().readString();
        key1_ = stream().readBytesNullable();
        key2_ = stream().readBytesNullable();
    }
    i32 v1_ = 0;
    std::optional<std::string> addr_;
    std::optional<std::vector<u8>> key1_, key2_;
};

// ---- 19002 UdpLatencyTestRequestMessage ----
class UdpLatencyTestRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19002; }
    const char* getMessageTypeName() const override { return "UdpLatencyTestRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
        if (payload_) {
            const auto& p = *payload_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
        payload_ = stream().readBytesNullable();
    }
    i32 v1_ = 0, v2_ = 0;
    std::optional<std::vector<u8>> payload_;
};

// ---- 29002 UdpLatencyTestResponseMessage ----
class UdpLatencyTestResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29002; }
    const char* getMessageTypeName() const override { return "UdpLatencyTestResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        if (payload_) {
            const auto& p = *payload_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        payload_ = stream().readBytesNullable();
    }
    i32 v1_ = 0;
    std::optional<std::vector<u8>> payload_;
};

// ---- 14211 UnbindFacebookAccountMessage: empty ----
class UnbindFacebookAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14211; }
    const char* getMessageTypeName() const override { return "UnbindFacebookAccountMessage"; }
};

// ---- 14213 UnbindKakaoAccountMessage: empty ----
class UnbindKakaoAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14213; }
    const char* getMessageTypeName() const override { return "UnbindKakaoAccountMessage"; }
};

// ---- 14214 UnbindLineAccountMessage: empty ----
class UnbindLineAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14214; }
    const char* getMessageTypeName() const override { return "UnbindLineAccountMessage"; }
};

// ---- 17337 UnbotifyReportMessage: bytes ----
class UnbotifyReportMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17337; }
    const char* getMessageTypeName() const override { return "UnbotifyReportMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (report_) {
            const auto& p = *report_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        report_ = stream().readBytesNullable();
    }
    std::optional<std::vector<u8>> report_;
};

// ---- 20133 UnlockAccountFailedMessage: int ----
class UnlockAccountFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20133; }
    const char* getMessageTypeName() const override { return "UnlockAccountFailedMessage"; }
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

// ---- 10121 UnlockAccountMessage ----
class UnlockAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10121; }
    const char* getMessageTypeName() const override { return "UnlockAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        accountId_.encode(stream());
        stream().writeString(s1_ ? &*s1_ : nullptr);
        stream().writeString(s2_ ? &*s2_ : nullptr);
        stream().writeString(s3_ ? &*s3_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountId_ = LogicLong::decode(stream());
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
    }
    LogicLong accountId_;
    std::optional<std::string> s1_, s2_, s3_;
};

// ---- 20132 UnlockAccountOkMessage ----
class UnlockAccountOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20132; }
    const char* getMessageTypeName() const override { return "UnlockAccountOkMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        accountId_.encode(stream());
        stream().writeString(s1_ ? &*s1_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountId_ = LogicLong::decode(stream());
        s1_ = stream().readString();
    }
    LogicLong accountId_;
    std::optional<std::string> s1_;
};

// ---- 12103 UpdatePlayerMapMessage ----
class UpdatePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12103; }
    const char* getMessageTypeName() const override { return "UpdatePlayerMapMessage"; }
    void encode() override;
    void decode() override;
    LogicLong mapId_;
    std::unique_ptr<LogicCompressedString> mapData_;
};

// ---- 22103 UpdatePlayerMapResponseMessage ----
class UpdatePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22103; }
    const char* getMessageTypeName() const override { return "UpdatePlayerMapResponseMessage"; }
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

// ---- 14116 ViewReplayByStringIdMessage: stringref ----
class ViewReplayByStringIdMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14116; }
    const char* getMessageTypeName() const override { return "ViewReplayByStringIdMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(replayId_);
    }
    void decode() override {
        PiranhaMessage::decode();
        replayId_ = stream().readStringReference();
    }
    std::string replayId_;
};

// ---- 24109 VisionUpdateMessage (BitStream payload, pending) ----
class VisionUpdateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24109; }
    const char* getMessageTypeName() const override { return "VisionUpdateMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
        stream().writeVInt(v3_);
        stream().writeVInt(v4_);
        stream().writeBoolean(b1_);
        stream().writeBoolean(hasExtra_);
        stream().writeVInt(hasExtra_ ? extra_ : 0);
        throw pending_reverse("VisionUpdateMessage needs BitStream");
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
        v3_ = stream().readVInt();
        v4_ = stream().readVInt();
        b1_ = stream().readBoolean();
        hasExtra_ = stream().readBoolean();
        extra_ = stream().readVInt();
        throw pending_reverse("VisionUpdateMessage needs BitStream");
    }
    i32 v1_ = 0, v2_ = 0, v3_ = 0, v4_ = 0;
    bool b1_ = false, hasExtra_ = false;
    i32 extra_ = 0;
};

// ---- 20173 YoozooBillingProcessedByServerMessage ----
class YoozooBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20173; }
    const char* getMessageTypeName() const override { return "YoozooBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeString(s1_ ? &*s1_ : nullptr);
        stream().writeString(s2_ ? &*s2_ : nullptr);
        stream().writeString(s3_ ? &*s3_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
    }
    i32 result_ = 0;
    std::optional<std::string> s1_, s2_, s3_;
};

// ---- 10523 YoozooBillingRequestMessage: stringref ----
class YoozooBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10523; }
    const char* getMessageTypeName() const override { return "YoozooBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(data_);
    }
    void decode() override {
        PiranhaMessage::decode();
        data_ = stream().readStringReference();
    }
    std::string data_;
};

// ---- 20523 YoozooOrderAvailableMessage ----
class YoozooOrderAvailableMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20523; }
    const char* getMessageTypeName() const override { return "YoozooOrderAvailableMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(static_cast<i32>(orders_.size()));
        for (const auto& o : orders_) stream().writeStringReference(o);
    }
    void decode() override {
        PiranhaMessage::decode();
        const i32 n = stream().readVInt();
        orders_.clear();
        for (i32 i = 0; i < n; ++i) orders_.push_back(stream().readStringReference());
    }
    std::vector<std::string> orders_;
};

} // namespace titan
