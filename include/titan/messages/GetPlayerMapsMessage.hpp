#pragma once

// GetPlayerMapsMessage (12102) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GetPlayerMapsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12102; }
    const char* getMessageTypeName() const override { return "GetPlayerMapsMessage"; }
};

} // namespace titan
