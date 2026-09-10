#pragma once

// TeamSetEventMessage (14362) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamSetEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14362; }
    const char* getMessageTypeName() const override { return "TeamSetEventMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
    }
    i32 v132_ = 0, v136_ = 0;
};

} // namespace titan
