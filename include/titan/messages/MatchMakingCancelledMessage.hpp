#pragma once

// MatchMakingCancelledMessage (20406) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class MatchMakingCancelledMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20406; }
    const char* getMessageTypeName() const override { return "MatchMakingCancelledMessage"; }
};

} // namespace titan
