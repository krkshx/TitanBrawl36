#pragma once

// LogicDeviceLinkMenuClosedMessage (16001) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicDeviceLinkMenuClosedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16001; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkMenuClosedMessage"; }
};

} // namespace titan
