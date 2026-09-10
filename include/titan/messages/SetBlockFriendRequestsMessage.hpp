#pragma once

// SetBlockFriendRequestsMessage (10576) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SetBlockFriendRequestsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10576; }
    const char* getMessageTypeName() const override { return "SetBlockFriendRequestsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(blocked_);
    }
    void decode() override {
        PiranhaMessage::decode();
        blocked_ = stream().readBoolean();
    }
    bool blocked_ = false;
};

} // namespace titan
