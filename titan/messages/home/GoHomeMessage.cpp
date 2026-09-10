#pragma once

// GoHomeMessage (14101) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GoHomeMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14101; }
    const char* getMessageTypeName() const override { return "GoHomeMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(flagA_);
        stream().writeBoolean(flagB_);
    }
    void decode() override {
        PiranhaMessage::decode();
        flagA_ = stream().readBoolean();
        flagB_ = stream().readBoolean();
    }
    bool flagA_ = false, flagB_ = false;
};

} // namespace titan
