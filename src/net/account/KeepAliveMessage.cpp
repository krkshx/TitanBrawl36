#pragma once
#include "../core/PiranhaMessage.cpp"

class KeepAliveMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10108; }
    std::int32_t getServiceNodeType() const override { return 1; }
};
