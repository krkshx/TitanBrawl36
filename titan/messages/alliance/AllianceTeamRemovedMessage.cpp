#pragma once

// AllianceTeamRemovedMessage (24365) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AllianceTeamRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24365; }
    const char* getMessageTypeName() const override { return "AllianceTeamRemovedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        teamId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        teamId_ = LogicLong::decode(stream());
    }
    LogicLong teamId_;
};

} // namespace titan
