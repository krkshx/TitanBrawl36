#pragma once

// PublishPlayerMapMessage (12105) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PublishPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12105; }
    const char* getMessageTypeName() const override { return "PublishPlayerMapMessage"; }
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
