#pragma once

// KakaoAccountUnboundMessage (24215) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class KakaoAccountUnboundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24215; }
    const char* getMessageTypeName() const override { return "KakaoAccountUnboundMessage"; }
};

} // namespace titan
