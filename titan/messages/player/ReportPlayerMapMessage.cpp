#pragma once

// ReportPlayerMapMessage (12125) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ReportPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12125; }
    const char* getMessageTypeName() const override { return "ReportPlayerMapMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        mapId_.encode(stream());
        stream().writeLongLong(v144_);
        stream().writeLongLong(v152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapId_ = LogicLong::decode(stream());
        v144_ = stream().readLongLong();
        v152_ = stream().readLongLong();
    }
    LogicLong mapId_;
    i64 v144_ = 0, v152_ = 0;
};

} // namespace titan
