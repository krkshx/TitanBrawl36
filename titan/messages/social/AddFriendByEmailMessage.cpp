#pragma once

// AddFriendByEmailMessage (10507) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AddFriendByEmailMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10507; }
    const char* getMessageTypeName() const override { return "AddFriendByEmailMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(email_ ? &*email_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        email_ = stream().readString();
    }
    std::optional<std::string> email_;
};

} // namespace titan
