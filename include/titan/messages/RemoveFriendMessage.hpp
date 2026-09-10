#pragma once

// RemoveFriendMessage (10506) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class RemoveFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10506; }
    const char* getMessageTypeName() const override { return "RemoveFriendMessage"; }
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
