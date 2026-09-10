#pragma once

// ReportPlayerMapResponseMessage (22125) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ReportPlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22125; }
    const char* getMessageTypeName() const override { return "ReportPlayerMapResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        mapId_.encode(stream());
        stream().writeString(note_ ? &*note_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        mapId_ = LogicLong::decode(stream());
        note_ = stream().readString();
    }
    i32 result_ = 0;
    LogicLong mapId_;
    std::optional<std::string> note_;
};

} // namespace titan
