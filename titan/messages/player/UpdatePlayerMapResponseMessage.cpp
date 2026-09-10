#pragma once

// UpdatePlayerMapResponseMessage (22103) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UpdatePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22103; }
    const char* getMessageTypeName() const override { return "UpdatePlayerMapResponseMessage"; }
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
