#pragma once

// AcceptTokenFriendMessage (13922) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AcceptTokenFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 13922; }
    const char* getMessageTypeName() const override { return "AcceptTokenFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeString(token_ ? &*token_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        token_ = stream().readString();
    }
    FriendAvatarBase base_;
    std::optional<std::string> token_;
};

} // namespace titan
