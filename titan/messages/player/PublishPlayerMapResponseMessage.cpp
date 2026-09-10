#pragma once

// PublishPlayerMapResponseMessage (22105) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class PublishPlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22105; }
    const char* getMessageTypeName() const override { return "PublishPlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        mapId_.encode(stream());
        stream().writeVInt(f144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        f144_ = stream().readVInt();
    }
    i32 result_ = 0, f144_ = 0;
    LogicLong mapId_;
};

} // namespace titan
