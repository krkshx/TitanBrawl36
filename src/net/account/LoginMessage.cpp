#pragma once
#include "../core/PiranhaMessage.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>
#include <string>

class LoginMessage : public PiranhaMessage {
public:
    LoginMessage() : PiranhaMessage(10) {}
    std::int32_t getMessageType() const override { return 10101; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "LoginMessage"; }

    void encode() override {
        ByteStream *s = getByteStream();
        LogicLong id;
        id.high = static_cast<std::int32_t>(accountId_ >> 32);
        id.low = static_cast<std::int32_t>(accountId_ & 0xFFFFFFFFll);
        s->writeLong(&id);
        s->writeString(&passToken_);
        s->writeInt(major_);
        s->writeInt(1);
        s->writeInt(build_);
        s->writeString(&resourceSha_);
        s->writeString(&deviceId_);
        ByteStreamHelper::writeDataReference(s, 1, 1);
        s->writeString(&language_);
        s->writeString(&advertisingId_);
        s->writeBoolean(isAndroid_);
        s->writeStringReference(deviceName_);
        s->writeStringReference(osVersion_);
        s->writeBoolean(isAdvertisingEnabled_);
        s->writeString(&androidId_);
        s->writeInt(rndKey_);
        s->writeVInt(0);
        s->writeStringReference(clientVersion_);
        s->writeStringReference(extra0_);
        s->writeStringReference(extra1_);
        s->writeVInt(0);
        s->writeStringReference(extra2_);
        s->writeStringReference(extra3_);
        s->writeStringReference(extra4_);
    }

    void decode() override {}

    void setAccountId(std::int64_t v) { accountId_ = v; }
    void setPassToken(const std::string &v) { passToken_ = v; }
    void setVersion(std::int32_t major, std::int32_t build) { major_ = major; build_ = build; }
    void setDevice(const std::string &id, const std::string &name, bool android) {
        deviceId_ = id;
        deviceName_ = name;
        isAndroid_ = android;
    }
    void setLanguage(const std::string &v) { language_ = v; }
    void setRndKey(std::int32_t v) { rndKey_ = v; }

private:
    std::int64_t accountId_ = 0;
    std::string passToken_;
    std::int32_t major_ = 36;
    std::int32_t build_ = 218;
    std::string resourceSha_;
    std::string deviceId_ = "titan-pc";
    std::string language_ = "en";
    std::string advertisingId_;
    bool isAndroid_ = true;
    std::string deviceName_;
    std::string osVersion_;
    bool isAdvertisingEnabled_ = false;
    std::string androidId_;
    std::int32_t rndKey_ = 0;
    std::string clientVersion_;
    std::string extra0_;
    std::string extra1_;
    std::string extra2_;
    std::string extra3_;
    std::string extra4_;
};
