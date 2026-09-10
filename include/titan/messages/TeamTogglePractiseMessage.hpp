#pragma once

// TeamTogglePractiseMessage (14356) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamTogglePractiseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14356; }
    const char* getMessageTypeName() const override { return "TeamTogglePractiseMessage"; }
};

} // namespace titan
