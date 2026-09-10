#pragma once

// TencentBillingProcessedByServerMessage (20153) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TencentBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20153; }
    const char* getMessageTypeName() const override { return "TencentBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
        stream().writeStringReference(f152_);
        stream().writeInt(f168_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
        f152_ = stream().readStringReference();
        f168_ = stream().readInt();
    }
    std::string f136_, f152_;
    i32 f168_ = 0;
};

} // namespace titan
