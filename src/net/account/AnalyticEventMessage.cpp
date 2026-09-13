#pragma once
#include "../core/PiranhaMessage.cpp"

class AnalyticEventMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10110; }
    std::int32_t getServiceNodeType() const override { return 9; }
};
