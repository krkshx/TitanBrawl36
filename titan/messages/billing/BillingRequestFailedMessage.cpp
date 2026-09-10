#pragma once

// BillingRequestFailedMessage (20121) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class BillingRequestFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20121; }
    const char* getMessageTypeName() const override { return "BillingRequestFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(errorCode_);
        stream().writeString(message_ ? &*message_ : nullptr);
        stream().writeInt(detail_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readInt();
        message_ = stream().readString();
        detail_ = stream().readInt();
    }
    i32 errorCode_ = 0, detail_ = 0;
    std::optional<std::string> message_;
};

} // namespace titan
