#pragma once

// TeamRequestJoinApproveMessage (14882) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamRequestJoinApproveMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14882; }
    const char* getMessageTypeName() const override { return "TeamRequestJoinApproveMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        playerId_.encode(stream());
        stream().writeBoolean(v144_);
        stream().writeBoolean(v145_);
    }
    void decode() override {
        PiranhaMessage::decode();
        playerId_ = LogicLong::decode(stream());
        v144_ = stream().readBoolean();
        v145_ = stream().readBoolean();
    }
    LogicLong playerId_;
    bool v144_ = false, v145_ = false;
};

} // namespace titan
