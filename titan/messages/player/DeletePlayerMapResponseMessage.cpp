#pragma once

// DeletePlayerMapResponseMessage (22101) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class DeletePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22101; }
    const char* getMessageTypeName() const override { return "DeletePlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        mapId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
    }
    i32 result_ = 0;
    LogicLong mapId_;
};

} // namespace titan
