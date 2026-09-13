#pragma once
#include "../core/PiranhaMessage.cpp"

class ServerHelloMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 20100; }
    std::int32_t getServiceNodeType() const override { return 1; }
    void encode() override {}
    void decode() override {}
};
