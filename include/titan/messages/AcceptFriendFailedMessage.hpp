#pragma once

// AcceptFriendFailedMessage (20501) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AcceptFriendFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20501; }
    const char* getMessageTypeName() const override { return "AcceptFriendFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        reason_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 reason_ = 0;
};

} // namespace titan
