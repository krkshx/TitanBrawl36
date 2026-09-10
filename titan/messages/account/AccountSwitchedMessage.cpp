#pragma once

// AccountSwitchedMessage (10118) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AccountSwitchedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10118; }
    const char* getMessageTypeName() const override { return "AccountSwitchedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        accountId_.encode(stream());
        stream().writeBoolean(flag1_);
        stream().writeBoolean(flag2_);
        stream().writeString(env_ ? &*env_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        accountId_ = LogicLong::decode(stream());
        flag1_ = stream().readBoolean();
        flag2_ = stream().readBoolean();
        env_ = stream().readString();
    }
    LogicLong accountId_;
    bool flag1_ = false, flag2_ = false;
    std::optional<std::string> env_;
};

} // namespace titan
