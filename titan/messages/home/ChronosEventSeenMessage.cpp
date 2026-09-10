#pragma once

// ChronosEventSeenMessage (14166) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ChronosEventSeenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14166; }
    const char* getMessageTypeName() const override { return "ChronosEventSeenMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(eventId_);
    }
    void decode() override {
        PiranhaMessage::decode();
        eventId_ = stream().readVInt();
    }
    i32 eventId_ = 0;
};

} // namespace titan
