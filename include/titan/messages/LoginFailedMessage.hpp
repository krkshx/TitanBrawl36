#pragma once

// LoginFailedMessage (20103) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

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

} // namespace titan
