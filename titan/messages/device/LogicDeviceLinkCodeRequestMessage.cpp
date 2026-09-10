#pragma once

// LogicDeviceLinkCodeRequestMessage (16000) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkCodeRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16000; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkCodeRequestMessage"; }
};

} // namespace titan
