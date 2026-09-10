#pragma once

// TencentBillingRequestMessage (10152) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TencentBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10152; }
    const char* getMessageTypeName() const override { return "TencentBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f1_);
        stream().writeStringReference(f2_);
        stream().writeStringReference(f4_);
        stream().writeStringReference(f3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readStringReference();
        f2_ = stream().readStringReference();
        f4_ = stream().readStringReference();
        f3_ = stream().readStringReference();
    }
    std::string f1_, f2_, f3_, f4_;
};

} // namespace titan
