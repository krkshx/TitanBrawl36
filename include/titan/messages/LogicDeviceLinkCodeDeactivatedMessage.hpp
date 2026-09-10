#pragma once

// LogicDeviceLinkCodeDeactivatedMessage (26004) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LogicDeviceLinkCodeDeactivatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 26004; }
    const char* getMessageTypeName() const override { return "LogicDeviceLinkCodeDeactivatedMessage"; }
};

} // namespace titan
