#pragma once

// CafeBazaarBillingRequestMessage (10153) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class CafeBazaarBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10153; }
    const char* getMessageTypeName() const override { return "CafeBazaarBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&s1_, &s2_, &s3_, &s4_, &s5_, &s6_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
        s4_ = stream().readString();
        s5_ = stream().readString();
        s6_ = stream().readString();
    }
    std::optional<std::string> s1_, s2_, s3_, s4_, s5_, s6_;
};

} // namespace titan
