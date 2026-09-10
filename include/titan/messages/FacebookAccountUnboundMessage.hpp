#pragma once

// FacebookAccountUnboundMessage (24214) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class FacebookAccountUnboundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24214; }
    const char* getMessageTypeName() const override { return "FacebookAccountUnboundMessage"; }
};

} // namespace titan
