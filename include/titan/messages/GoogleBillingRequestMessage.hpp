#pragma once

// GoogleBillingRequestMessage (10151) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GoogleBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10151; }
    const char* getMessageTypeName() const override { return "GoogleBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&f1_, &f2_, &f3_, &f4_, &f5_, &f6_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readString();
        f2_ = stream().readString();
        f3_ = stream().readString();
        f4_ = stream().readString();
        f5_ = stream().readString();
        f6_ = stream().readString();
    }
    std::optional<std::string> f1_, f2_, f3_, f4_, f5_, f6_;
};

} // namespace titan
