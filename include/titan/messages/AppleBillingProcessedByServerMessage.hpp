#pragma once

// AppleBillingProcessedByServerMessage (20151) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AppleBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20151; }
    const char* getMessageTypeName() const override { return "AppleBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(f1_ ? &*f1_ : nullptr);
        stream().writeString(f2_ ? &*f2_ : nullptr);
        stream().writeInt(f3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readString();
        f2_ = stream().readString();
        f3_ = stream().readInt();
    }
    std::optional<std::string> f1_, f2_;
    i32 f3_ = 0;
};

} // namespace titan
