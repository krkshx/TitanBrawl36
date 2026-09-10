#pragma once

// AddFriendMessage (10502) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AddFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10502; }
    const char* getMessageTypeName() const override { return "AddFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(avatarIdHigh_);
        stream().writeInt(avatarIdLow_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        avatarIdHigh_ = stream().readInt();
        avatarIdLow_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 avatarIdHigh_ = 0, avatarIdLow_ = 0;
};

} // namespace titan
