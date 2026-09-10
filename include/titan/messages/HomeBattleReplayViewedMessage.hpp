#pragma once

// HomeBattleReplayViewedMessage (24117) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class HomeBattleReplayViewedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24117; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayViewedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(replaySlot_);
        playerId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        replaySlot_ = stream().readVInt();
        playerId_ = LogicLong::decode(stream());
    }
    i32 replaySlot_ = 0;
    LogicLong playerId_;
};

} // namespace titan
