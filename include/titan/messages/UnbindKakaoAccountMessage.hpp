#pragma once

// UnbindKakaoAccountMessage (14213) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class UnbindKakaoAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14213; }
    const char* getMessageTypeName() const override { return "UnbindKakaoAccountMessage"; }
};

} // namespace titan
