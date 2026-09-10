#pragma once

// LeaveAllianceMessage (14308) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LeaveAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14308; }
    const char* getMessageTypeName() const override { return "LeaveAllianceMessage"; }
};

} // namespace titan
