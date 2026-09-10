#pragma once

// UdpCheckConnectionMessage (10109) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UdpCheckConnectionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10109; }
    const char* getMessageTypeName() const override { return "UdpCheckConnectionMessage"; }
};

} // namespace titan
