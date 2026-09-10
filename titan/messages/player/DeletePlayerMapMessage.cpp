#pragma once

// DeletePlayerMapMessage (12101) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class DeletePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12101; }
    const char* getMessageTypeName() const override { return "DeletePlayerMapMessage"; }
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
