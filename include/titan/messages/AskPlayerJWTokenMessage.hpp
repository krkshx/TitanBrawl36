#pragma once

// AskPlayerJWTokenMessage (10055) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AskPlayerJWTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10055; }
    const char* getMessageTypeName() const override { return "AskPlayerJWTokenMessage"; }
};

} // namespace titan
