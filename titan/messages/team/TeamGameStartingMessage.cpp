#pragma once

// TeamGameStartingMessage (24130) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamGameStartingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24130; }
    const char* getMessageTypeName() const override { return "TeamGameStartingMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(timerSec_);
        stream().writeVInt(unk134_);
        stream().writeVInt(unk135_);
    }
    void decode() override {
        PiranhaMessage::decode();
        timerSec_ = stream().readVInt();
        unk134_ = stream().readVInt();
        unk135_ = stream().readVInt();
    }
    i32 timerSec_ = 0, unk134_ = 0, unk135_ = 0;
};

} // namespace titan
