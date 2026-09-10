#pragma once

// BattleLogViewReplayMessage (14115) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class BattleLogViewReplayMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14115; }
    const char* getMessageTypeName() const override { return "BattleLogViewReplayMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeLongLong(q136_);
        stream().writeLongLong(q144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        q136_ = stream().readLongLong();
        q144_ = stream().readLongLong();
    }
    i64 q136_ = 0, q144_ = 0;
};

} // namespace titan
