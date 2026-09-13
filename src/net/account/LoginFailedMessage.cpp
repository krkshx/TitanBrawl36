#pragma once
#include "../core/PiranhaMessage.cpp"
#include <string>

class LoginFailedMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 20103; }
    std::int32_t getServiceNodeType() const override { return 1; }
    void encode() override {}
    void decode() override {}
    std::int32_t errorCode = 0;
    std::string fingerprintSha;
    std::string message;
    std::string updateUrl;
    std::string contentUrl;
};
