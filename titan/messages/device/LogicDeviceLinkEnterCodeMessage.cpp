#pragma once

// LogicDeviceLinkEnterCodeMessage (16002) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkEnterCodeMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16002; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkEnterCodeMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(code_);
    }
    void decode() override {
        PiranhaMessage::decode();
        code_ = stream().readStringReference();
    }
    std::string code_;
};

} // namespace titan
