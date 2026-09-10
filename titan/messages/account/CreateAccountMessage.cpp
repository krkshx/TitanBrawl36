#pragma once

// CreateAccountMessage (10103) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class CreateAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10103; }
    const char* getMessageTypeName() const override { return "CreateAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (auto* s : {&s1_, &s2_, &s3_, &s4_, &s5_})
            stream().writeString(*s ? &**s : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
        s4_ = stream().readString();
        s5_ = stream().readString();
    }
    std::optional<std::string> s1_, s2_, s3_, s4_, s5_;
};

} // namespace titan
