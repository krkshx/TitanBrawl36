#pragma once
#include "../core/PiranhaMessage.cpp"

class OwnHomeDataMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 24101; }
    std::int32_t getServiceNodeType() const override { return 9; }
    void encode() override {}
    void decode() override {}
};
