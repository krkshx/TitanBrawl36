#pragma once

// YoozooBillingProcessedByServerMessage (20173) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class YoozooBillingProcessedByServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20173; }
    const char* getMessageTypeName() const override { return "YoozooBillingProcessedByServerMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeString(s1_ ? &*s1_ : nullptr);
        stream().writeString(s2_ ? &*s2_ : nullptr);
        stream().writeString(s3_ ? &*s3_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
    }
    i32 result_ = 0;
    std::optional<std::string> s1_, s2_, s3_;
};

} // namespace titan
