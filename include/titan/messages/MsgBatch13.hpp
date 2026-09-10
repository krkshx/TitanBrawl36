#pragma once

// Message batch 13 — reversed from libg_decrypted.so (ARM64).
// Single class: YoozooOrderDeliveryFailed.

#include "titan/core/PiranhaMessage.hpp"

#include <optional>
#include <string>

namespace titan {

// ---- 20545 YoozooOrderDeliveryFailedMessage ----
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
