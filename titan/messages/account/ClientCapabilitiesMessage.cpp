#pragma once

// ClientCapabilitiesMessage (10107) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ClientCapabilitiesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10107; }
    const char* getMessageTypeName() const override { return "ClientCapabilitiesMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(flags_);
    }
    void decode() override {
        PiranhaMessage::decode();
        flags_ = stream().readVInt();
    }
    i32 flags_ = 0;
};

} // namespace titan
