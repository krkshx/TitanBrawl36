#pragma once

// GetAllianceInviteTokenMessage (10309) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GetAllianceInviteTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10309; }
    const char* getMessageTypeName() const override { return "GetAllianceInviteTokenMessage"; }
};

} // namespace titan
