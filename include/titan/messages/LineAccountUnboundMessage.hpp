#pragma once

// LineAccountUnboundMessage (24216) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LineAccountUnboundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24216; }
    const char* getMessageTypeName() const override { return "LineAccountUnboundMessage"; }
};

} // namespace titan
