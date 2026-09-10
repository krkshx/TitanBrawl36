#pragma once

// RequestMatchCancelMessage (14117) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RequestMatchCancelMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14117; }
    const char* getMessageTypeName() const override { return "RequestMatchCancelMessage"; }
};

} // namespace titan
