#pragma once

// AppleBillingRequestMessage (10150) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AppleBillingRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10150; }
    const char* getMessageTypeName() const override { return "AppleBillingRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&s1_, &s2_, &s3_, &s4_})
            stream().writeString(*s ? &**s : nullptr);
        if (payload_) {
            const auto& p = *payload_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
        s4_ = stream().readString();
        payload_ = stream().readBytesNullable();
    }
    std::optional<std::string> s1_, s2_, s3_, s4_;
    std::optional<std::vector<u8>> payload_;
};

} // namespace titan
