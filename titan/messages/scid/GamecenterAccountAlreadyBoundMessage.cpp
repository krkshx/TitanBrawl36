#pragma once

// GamecenterAccountAlreadyBoundMessage (22687) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GamecenterAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 22687; }
    const char* getMessageTypeName() const override { return "GamecenterAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(true, true);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(true, true);
    }
};

} // namespace titan
