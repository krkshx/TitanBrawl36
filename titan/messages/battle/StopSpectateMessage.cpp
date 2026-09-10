#pragma once

// StopSpectateMessage (14107) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class StopSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14107; }
    const char* getMessageTypeName() const override { return "StopSpectateMessage"; }
};

} // namespace titan
