#pragma once
#include "../core/PiranhaMessage.cpp"
#include <string>

class LoginMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10101; }
    std::int32_t getServiceNodeType() const override { return 1; }
    void encode() override {}
    void decode() override {}
    std::int32_t highId = 0;
    std::int32_t lowId = 0;
    std::string passToken;
    std::int32_t major = 0;
    std::int32_t minor = 0;
    std::int32_t build = 0;
    std::string resourceSha;
    std::string udid;
    std::string openUdid;
    std::string macAddress;
    std::string device;
    std::string advertisingId;
    std::int32_t osVersion = 0;
    bool isAndroid = false;
    std::string imei;
    std::string androidId;
    bool isAdvertisingEnabled = false;
    std::string preferredLanguageId;
};
