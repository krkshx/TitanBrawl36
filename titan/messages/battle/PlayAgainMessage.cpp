#pragma once

// PlayAgainMessage (14177) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PlayAgainMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14177; }
    const char* getMessageTypeName() const override { return "PlayAgainMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(f132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readBoolean();
    }
    bool f132_ = false;
};

} // namespace titan
