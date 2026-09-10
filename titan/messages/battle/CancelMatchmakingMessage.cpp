#pragma once

// CancelMatchmakingMessage (14106) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class CancelMatchmakingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14106; }
    const char* getMessageTypeName() const override { return "CancelMatchmakingMessage"; }
};

} // namespace titan
