#pragma once

// ReportUserMessage (10117) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ReportUserMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10117; }
    const char* getMessageTypeName() const override { return "ReportUserMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(reason_);
        playerId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        reason_ = stream().readInt();
        playerId_ = LogicLong::decode(stream());
    }
    i32 reason_ = 0;
    LogicLong playerId_;
};

} // namespace titan
