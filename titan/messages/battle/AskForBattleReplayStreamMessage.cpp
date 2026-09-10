#pragma once

// AskForBattleReplayStreamMessage (14406) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AskForBattleReplayStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14406; }
    const char* getMessageTypeName() const override { return "AskForBattleReplayStreamMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        battleId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        battleId_ = LogicLong::decode(stream());
    }
    LogicLong battleId_;
};

} // namespace titan
