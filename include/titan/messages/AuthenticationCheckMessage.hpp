#pragma once

// AuthenticationCheckMessage (10112) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AuthenticationCheckMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10112; }
    const char* getMessageTypeName() const override { return "AuthenticationCheckMessage"; }
};

} // namespace titan
