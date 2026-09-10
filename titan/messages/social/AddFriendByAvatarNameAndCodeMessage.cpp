#pragma once

// AddFriendByAvatarNameAndCodeMessage (10509) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AddFriendByAvatarNameAndCodeMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10509; }
    const char* getMessageTypeName() const override { return "AddFriendByAvatarNameAndCodeMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(name_ ? &*name_ : nullptr);
        stream().writeString(code_ ? &*code_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        name_ = stream().readString();
        code_ = stream().readString();
    }
    std::optional<std::string> name_, code_;
};

} // namespace titan
