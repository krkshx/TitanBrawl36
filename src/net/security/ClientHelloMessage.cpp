#pragma once
#include "../core/PiranhaMessage.cpp"

class ClientHelloMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10100; }
    std::int32_t getServiceNodeType() const override { return 1; }
    void encode() override {}
    void decode() override {}
};
