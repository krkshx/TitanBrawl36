#pragma once

// CreateGameMessage (10401) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class CreateGameMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10401; }
    const char* getMessageTypeName() const override { return "CreateGameMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(f1_);
        stream().writeInt(f2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f1_ = stream().readInt();
        f2_ = stream().readInt();
    }
    i32 f1_ = 0, f2_ = 0;
};

} // namespace titan
