#pragma once

// GoHomeFromSpectateMessage (14108) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GoHomeFromSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14108; }
    const char* getMessageTypeName() const override { return "GoHomeFromSpectateMessage"; }
};

} // namespace titan
