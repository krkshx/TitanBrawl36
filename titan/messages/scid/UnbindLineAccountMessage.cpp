#pragma once

// UnbindLineAccountMessage (14214) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UnbindLineAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14214; }
    const char* getMessageTypeName() const override { return "UnbindLineAccountMessage"; }
};

} // namespace titan
