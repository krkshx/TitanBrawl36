#pragma once

// Message batch 14 — classes present in LogicLaserMessageFactory (0x513e58)
// but missed by the first index sweep. Reversed from libg_decrypted.so.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/core/PiranhaMessage.hpp"
#include "titan/messages/MsgBatch00.hpp" // FriendAvatarBase
#include "titan/messages/Nested.hpp"

#include <memory>
#include <string>
#include <vector>

namespace titan {

// ---- 10101 LoginMessage (ENC 0x483cd0, DEC 0x467f78) ----
class LoginMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10101; }
    const char* getMessageTypeName() const override { return "LoginMessage"; }
    void encode() override;
    void decode() override;
    LogicLong accountId_;
    std::optional<std::string> s1_;
    i32 i1_ = 0, i2_ = 0, i3_ = 0;
    std::optional<std::string> s2_, s3_;
    std::optional<DataReference> ref_;
    std::optional<std::string> s4_, s5_;
    bool b1_ = false;
    std::string r1_, r2_;
    bool b2_ = false;
    std::optional<std::string> s6_;
    i32 i4_ = 0, v1_ = 0;
    std::string r3_, r4_, r5_;
    i32 v2_ = 0;
    std::string r6_, r7_, r8_;
    std::optional<std::vector<u8>> payload_;
    bool b3_ = false;
    std::string r9_, r10_;
};

// ---- 10501 AcceptFriendMessage: base + int + int ----
class AcceptFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10501; }
    const char* getMessageTypeName() const override { return "AcceptFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(f1_);
        stream().writeInt(f2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        f1_ = stream().readInt();
        f2_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 f1_ = 0, f2_ = 0;
};

// ---- 10504 AskForFriendListMessage: empty ----
class AskForFriendListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10504; }
    const char* getMessageTypeName() const override { return "AskForFriendListMessage"; }
};

// ---- 10506 RemoveFriendMessage: base + int + int ----
class RemoveFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10506; }
    const char* getMessageTypeName() const override { return "RemoveFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(f1_);
        stream().writeInt(f2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        f1_ = stream().readInt();
        f2_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 f1_ = 0, f2_ = 0;
};

// ---- 10599 AskForFriendSuggestionsMessage: empty ----
class AskForFriendSuggestionsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10599; }
    const char* getMessageTypeName() const override { return "AskForFriendSuggestionsMessage"; }
};

// ---- 15793 GetTokenFriendMessage: empty ----
class GetTokenFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 15793; }
    const char* getMessageTypeName() const override { return "GetTokenFriendMessage"; }
};

// ---- 10309 GetAllianceInviteTokenMessage: empty ----
class GetAllianceInviteTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10309; }
    const char* getMessageTypeName() const override { return "GetAllianceInviteTokenMessage"; }
};

// ---- 20103 LoginFailedMessage (ENC 0x3adb20, DEC 0x29f5d8) ----
class LoginFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20103; }
    const char* getMessageTypeName() const override { return "LoginFailedMessage"; }
    void encode() override;
    void decode() override;
    i32 code_ = 0;
    std::optional<std::string> s1_, s2_, s3_, s4_, s5_;
    i32 i1_ = 0;
    bool b1_ = false;
    std::optional<std::vector<u8>> payload_;
    std::vector<std::string> list_; // int count (-1=null) + strings
    bool listNull_ = false;
    i32 i2_ = 0, i3_ = 0;
    std::optional<std::string> s6_;
    i32 i4_ = 0;
    bool b2_ = false, b3_ = false;
    // Optional tail (isAtEnd-guarded on decode):
    std::string r1_;
    i32 v1_ = 0;
    std::string r2_;
    bool b4_ = false;
    LogicLong tailId_;
    bool hasTail_ = false;
};

// ---- 20000 ExtendedSetEncryptionMessage: bytes + int ----
// NOTE: same numeric id as SetEncryptionMessage (20000); factory
// disambiguation pending (probably message version).
class ExtendedSetEncryptionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20000; }
    const char* getMessageTypeName() const override { return "ExtendedSetEncryptionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (key_) {
            const auto& p = *key_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
        stream().writeInt(tail_);
    }
    void decode() override {
        PiranhaMessage::decode();
        key_ = stream().readBytesNullable();
        tail_ = stream().readInt();
    }
    std::optional<std::vector<u8>> key_;
    i32 tail_ = 0;
};

} // namespace titan
