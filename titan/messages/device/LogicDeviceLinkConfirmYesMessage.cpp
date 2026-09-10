#pragma once

// LogicDeviceLinkConfirmYesMessage (16003) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkConfirmYesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16003; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkConfirmYesMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(f136_);
        stream().writeStringReference(f160_);
        stream().writeStringReference(f176_);
        id_.encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        f136_ = stream().readStringReference();
        f160_ = stream().readStringReference();
        f176_ = stream().readStringReference();
        id_ = LogicLong::decode(stream());
    }
    std::string f136_, f160_, f176_;
    LogicLong id_;
};

} // namespace titan
