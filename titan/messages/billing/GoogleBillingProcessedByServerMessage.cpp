#pragma once

// GoogleBillingProcessedByServerMessage (20152) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GoogleBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20152; }
    const char* getMessageTypeName() const override { return "GoogleBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(receipt_ ? &*receipt_ : nullptr);
        stream().writeString(signature_ ? &*signature_ : nullptr);
        stream().writeInt(resultCode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        receipt_ = stream().readString();
        signature_ = stream().readString();
        resultCode_ = stream().readInt();
    }
    std::optional<std::string> receipt_, signature_;
    i32 resultCode_ = 0;
};

} // namespace titan
