#pragma once

// TeamMemberStatusMessage (14361) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamMemberStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14361; }
    const char* getMessageTypeName() const override { return "TeamMemberStatusMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
    }
    i32 v132_ = 0;
};

} // namespace titan
