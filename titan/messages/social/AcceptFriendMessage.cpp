#pragma once

// AcceptFriendMessage (10501) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AcceptFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10501; }
    const char* getMessageTypeName() const override { return "AcceptFriendMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        base_.encode(stream());
        stream().writeInt(f1_);
        stream().writeInt(f2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        base_.decode(stream());
        f1_ = stream().readInt();
        f2_ = stream().readInt();
    }
    FriendAvatarBase base_;
    i32 f1_ = 0, f2_ = 0;
};

} // namespace titan
