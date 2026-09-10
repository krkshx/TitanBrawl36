#pragma once

// RespondToAllianceJoinRequestMessage (14321) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class RespondToAllianceJoinRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14321; }
    const char* getMessageTypeName() const override { return "RespondToAllianceJoinRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeBoolean(accept_);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        accept_ = stream().readBoolean();
    }
    LogicLong playerId_;
    bool accept_ = false;
};

} // namespace titan
