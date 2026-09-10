#pragma once

// UnbindFacebookAccountMessage (14211) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class UnbindFacebookAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14211; }
    const char* getMessageTypeName() const override { return "UnbindFacebookAccountMessage"; }
};

} // namespace titan
