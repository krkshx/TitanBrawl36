#pragma once

// HomeBattleReplayMessage (14114) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class HomeBattleReplayMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14114; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(replaySlot_);
        sharerId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        replaySlot_ = stream().readInt();
        sharerId_ = LogicLong::decode(stream());
    }
    i32 replaySlot_ = 0;
    LogicLong sharerId_;
};

} // namespace titan
