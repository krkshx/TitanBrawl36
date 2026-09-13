#pragma once
#include "../core/PiranhaMessage.cpp"

class KeepAliveServerMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 20108; }
    std::int32_t getServiceNodeType() const override { return 1; }
};
