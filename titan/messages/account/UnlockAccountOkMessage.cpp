#pragma once

// UnlockAccountOkMessage (20132) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UnlockAccountOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20132; }
    const char* getMessageTypeName() const override { return "UnlockAccountOkMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        accountId_.encode(stream());
        stream().writeString(s1_ ? &*s1_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountId_ = LogicLong::decode(stream());
        s1_ = stream().readString();
    }
    LogicLong accountId_;
    std::optional<std::string> s1_;
};

} // namespace titan
