#pragma once

// AuthenticationResponseMessage (20104) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AuthenticationResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20104; }
    const char* getMessageTypeName() const override { return "AuthenticationResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(success_);
    }
    void decode() override {
        PiranhaMessage::decode();
        success_ = stream().readBoolean();
    }
    bool success_ = false;
};

} // namespace titan
