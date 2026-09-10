#pragma once

// YoozooOrderDeliveryFailedMessage (20545) — split out of MsgBatch13.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class YoozooOrderDeliveryFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20545; }
    const char* getMessageTypeName() const override { return "YoozooOrderDeliveryFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(orderId_ ? &*orderId_ : nullptr);
        stream().writeVInt(errorCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        orderId_ = stream().readString();
        errorCode_ = stream().readVInt();
    }
    std::optional<std::string> orderId_;
    i32 errorCode_ = 0;
};

} // namespace titan
