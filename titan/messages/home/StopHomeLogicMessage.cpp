#pragma once

// StopHomeLogicMessage (24106) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class StopHomeLogicMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24106; }
    const char* getMessageTypeName() const override { return "StopHomeLogicMessage"; }
};

} // namespace titan
