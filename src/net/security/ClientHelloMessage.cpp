#pragma once
#include "../core/PiranhaMessage.cpp"
#include <string>

// Раскладка строго по либе (сеттеры -> смещения):
// 132 protocol (pepper: 1..3, см. protocolIsPepper @ 0x5f685c), 136 keyVersion
// (= PepperKey::VERSION = 24), 140 major, 144 minor, 148 build,
// 152 contentHash (stringref), 168 deviceType, 172 appStore.
// Значения орига из ServerConnection::update @ 0x6bbb2c:
// protocol=2, keyVersion=24, major=36, minor=0, build=218, deviceType=2, appStore=2.
class ClientHelloMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10100; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "ClientHelloMessage"; }

    void encode() override {
        ByteStream *s = getByteStream();
        s->writeInt(protocol_);
        s->writeInt(keyVersion_);
        s->writeInt(major_);
        s->writeInt(minor_);
        s->writeInt(build_);
        s->writeStringReference(contentHash_);
        s->writeInt(deviceType_);
        s->writeInt(appStore_);
    }

    void decode() override {
        ByteStream *s = getByteStream();
        protocol_ = s->readInt();
        keyVersion_ = s->readInt();
        major_ = s->readInt();
        minor_ = s->readInt();
        build_ = s->readInt();
        contentHash_ = s->readStringReference(900000);
        deviceType_ = s->readInt();
        appStore_ = s->readInt();
    }

    void setProtocol(std::int32_t v) { protocol_ = v; }
    void setKeyVersion(std::int32_t v) { keyVersion_ = v; }
    void setMajorVersion(std::int32_t v) { major_ = v; }
    void setMinorVersion(std::int32_t v) { minor_ = v; }
    void setBuild(std::int32_t v) { build_ = v; }
    void setContentHash(const std::string &v) { contentHash_ = v; }
    void setDeviceType(std::int32_t v) { deviceType_ = v; }
    void setAppStore(std::int32_t v) { appStore_ = v; }

private:
    std::int32_t protocol_ = 2;
    std::int32_t keyVersion_ = 24;
    std::int32_t major_ = 36;
    std::int32_t minor_ = 0;
    std::int32_t build_ = 218;
    std::string contentHash_;
    std::int32_t deviceType_ = 2;
    std::int32_t appStore_ = 2;
};
