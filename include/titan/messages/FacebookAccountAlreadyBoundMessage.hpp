#pragma once

// FacebookAccountAlreadyBoundMessage (24202) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class FacebookAccountAlreadyBoundMessage : public AccountAlreadyBoundBase {
public:
    int getMessageType() const override { return 24202; }
    const char* getMessageTypeName() const override { return "FacebookAccountAlreadyBoundMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeBound(false, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeBound(false, false);
    }
};

} // namespace titan
