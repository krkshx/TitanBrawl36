#pragma once

// SubmitPlayerMapResponseMessage (22104) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SubmitPlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22104; }
    const char* getMessageTypeName() const override { return "SubmitPlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(resultCode_);
        mapId_.encode(stream());
        stream().writeVInt(unk144_);
    }
    void decode() override {
        PiranhaMessage::decode();
        resultCode_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        unk144_ = stream().readVInt();
    }
    i32 resultCode_ = 0, unk144_ = 0;
    LogicLong mapId_;
};

} // namespace titan
