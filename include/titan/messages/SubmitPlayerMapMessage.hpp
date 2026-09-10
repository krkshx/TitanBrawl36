#pragma once

// SubmitPlayerMapMessage (12104) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SubmitPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12104; }
    const char* getMessageTypeName() const override { return "SubmitPlayerMapMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
    }
    LogicLong mapId_;
};

} // namespace titan
