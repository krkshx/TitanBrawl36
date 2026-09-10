#pragma once

// TeamRequestJoinCancelMessage (14880) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamRequestJoinCancelMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14880; }
    const char* getMessageTypeName() const override { return "TeamRequestJoinCancelMessage"; }
};

} // namespace titan
