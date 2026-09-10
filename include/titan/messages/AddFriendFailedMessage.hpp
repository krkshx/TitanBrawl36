#pragma once

// AddFriendFailedMessage (20112) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AddFriendFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20112; }
    const char* getMessageTypeName() const override { return "AddFriendFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
    }
    i32 reason_ = 0;
};

} // namespace titan
