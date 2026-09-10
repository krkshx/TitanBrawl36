#pragma once

// TeamKickMessage (14352) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamKickMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14352; }
    const char* getMessageTypeName() const override { return "TeamKickMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(slotId_);
        stream().writeVInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        slotId_ = stream().readVInt();
        reason_ = stream().readVInt();
    }
    i32 slotId_ = 0, reason_ = 0;
};

} // namespace titan
