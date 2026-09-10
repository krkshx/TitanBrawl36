#pragma once

// TeamLeaveMessage (14353) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamLeaveMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14353; }
    const char* getMessageTypeName() const override { return "TeamLeaveMessage"; }
};

} // namespace titan
