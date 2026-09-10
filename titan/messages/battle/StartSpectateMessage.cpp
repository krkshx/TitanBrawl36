#pragma once

// StartSpectateMessage (14104) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class StartSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14104; }
    const char* getMessageTypeName() const override { return "StartSpectateMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeBoolean(allowSpectate_);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        allowSpectate_ = stream().readBoolean();
    }
    LogicLong playerId_;
    bool allowSpectate_ = false;
};

} // namespace titan
