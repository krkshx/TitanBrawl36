#pragma once

// LookForGameRoomRequestMessage (14199) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LookForGameRoomRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14199; }
    const char* getMessageTypeName() const override { return "LookForGameRoomRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(f132_);
        stream().writeVInt(f136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readVInt();
        f136_ = stream().readVInt();
    }
    i32 f132_ = 0, f136_ = 0;
};

} // namespace titan
