#pragma once

// KunlunBillingProcessedByServerMessage (20156) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class KunlunBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20156; }
    const char* getMessageTypeName() const override { return "KunlunBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(receipt_);
        stream().writeStringReference(signature_);
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        receipt_ = stream().readStringReference();
        signature_ = stream().readStringReference();
        resultCode_ = stream().readInt();
    }
    std::string receipt_, signature_;
    i32 resultCode_ = 0;
};

} // namespace titan
