#pragma once

// YoozooBillingRequestMessage (10523) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

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

} // namespace titan
