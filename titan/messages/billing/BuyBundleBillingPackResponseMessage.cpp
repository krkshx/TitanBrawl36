#pragma once

// BuyBundleBillingPackResponseMessage (28363) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class BuyBundleBillingPackResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28363; }
    const char* getMessageTypeName() const override { return "BuyBundleBillingPackResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
    }
    i32 v132_ = 0, v136_ = 0;
};

} // namespace titan
