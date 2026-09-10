#pragma once

// HomeBattleReplayFailedMessage (24116) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class HomeBattleReplayFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24116; }
    const char* getMessageTypeName() const override { return "HomeBattleReplayFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
    }
    i32 reason_ = 0;
};

} // namespace titan
