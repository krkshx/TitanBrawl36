#pragma once

// AllianceMemberRemovedMessage (24309) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AllianceMemberRemovedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24309; }
    const char* getMessageTypeName() const override { return "AllianceMemberRemovedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        avatarId_.encode(stream());
        allianceId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        avatarId_ = LogicLong::decode(stream());
        allianceId_ = LogicLong::decode(stream());
    }
    LogicLong avatarId_, allianceId_;
};

} // namespace titan
