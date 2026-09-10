#pragma once

// BattleLogReplayAvailableMessage (23459) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class BattleLogReplayAvailableMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23459; }
    const char* getMessageTypeName() const override { return "BattleLogReplayAvailableMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeLongLong(q136_);
        stream().writeLongLong(q144_);
        stream().writeVInt(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        q136_ = stream().readLongLong();
        q144_ = stream().readLongLong();
        v132_ = stream().readVInt();
    }
    i64 q136_ = 0, q144_ = 0;
    i32 v132_ = 0;
};

} // namespace titan
