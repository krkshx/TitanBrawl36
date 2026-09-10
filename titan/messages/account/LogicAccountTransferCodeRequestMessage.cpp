#pragma once

// LogicAccountTransferCodeRequestMessage (17000) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LogicAccountTransferCodeRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17000; }
    const char* getMessageTypeName() const override { return "LogicAccountTransferCodeRequestMessage"; }
};

} // namespace titan
