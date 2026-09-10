#pragma once

// LogicDeviceLinkNewDeviceLinkedMessage (26003) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkNewDeviceLinkedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26003; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkNewDeviceLinkedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
    }
    std::string f136_;
};

} // namespace titan
