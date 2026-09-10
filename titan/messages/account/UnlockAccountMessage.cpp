#pragma once

// UnlockAccountMessage (10121) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UnlockAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10121; }
    const char* getMessageTypeName() const override { return "UnlockAccountMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        accountId_.encode(stream());
        stream().writeString(s1_ ? &*s1_ : nullptr);
        stream().writeString(s2_ ? &*s2_ : nullptr);
        stream().writeString(s3_ ? &*s3_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountId_ = LogicLong::decode(stream());
        s1_ = stream().readString();
        s2_ = stream().readString();
        s3_ = stream().readString();
    }
    LogicLong accountId_;
    std::optional<std::string> s1_, s2_, s3_;
};

} // namespace titan
