#pragma once

// CafeBazaarBillingProcessedByServerMessage (20154) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class CafeBazaarBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20154; }
    const char* getMessageTypeName() const override { return "CafeBazaarBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(receipt_ ? &*receipt_ : nullptr);
        stream().writeString(token_ ? &*token_ : nullptr);
        stream().writeInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        receipt_ = stream().readString();
        token_ = stream().readString();
        result_ = stream().readInt();
    }
    std::optional<std::string> receipt_, token_;
    i32 result_ = 0;
};

} // namespace titan
