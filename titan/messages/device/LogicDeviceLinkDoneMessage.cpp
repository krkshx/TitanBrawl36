#pragma once

// LogicDeviceLinkDoneMessage (26007) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkDoneMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26007; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkDoneMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f144_);
        id_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        f144_ = stream().readStringReference();
        id_ = LogicLong::decode(stream());
    }
    std::string f144_;
    LogicLong id_;
};

} // namespace titan
