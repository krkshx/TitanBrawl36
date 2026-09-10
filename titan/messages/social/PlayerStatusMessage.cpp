#pragma once

// PlayerStatusMessage (14366) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PlayerStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14366; }
    const char* getMessageTypeName() const override { return "PlayerStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(status_);
    }
    void decode() override {
        PiranhaMessage::decode();
        status_ = stream().readVInt();
    }
    i32 status_ = 0;
};

} // namespace titan
