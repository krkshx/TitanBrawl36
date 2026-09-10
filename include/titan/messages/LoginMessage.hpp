#pragma once

// LoginMessage (10101) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

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

} // namespace titan
