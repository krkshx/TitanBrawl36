#pragma once

// SpectateFailedMessage (24105) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SpectateFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24105; }
    const char* getMessageTypeName() const override { return "SpectateFailedMessage"; }
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
