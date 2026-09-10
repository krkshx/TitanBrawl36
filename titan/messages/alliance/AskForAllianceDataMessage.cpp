#pragma once

// AskForAllianceDataMessage (14302) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AskForAllianceDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14302; }
    const char* getMessageTypeName() const override { return "AskForAllianceDataMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        allianceId_.encode(stream());
        stream().writeBoolean(hasSecond_);
        if (hasSecond_) secondId_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        allianceId_ = LogicLong::decode(stream());
        hasSecond_ = stream().readBoolean();
        if (hasSecond_) secondId_ = LogicLong::decode(stream());
    }
    LogicLong allianceId_, secondId_;
    bool hasSecond_ = false;
};

} // namespace titan
