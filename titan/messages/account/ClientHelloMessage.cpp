#pragma once

// ClientHelloMessage (10100) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ClientHelloMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10100; }
    const char* getMessageTypeName() const override { return "ClientHelloMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(protocolVersion_);
        stream().writeInt(keyVersion_);
        stream().writeInt(majorVersion_);
        stream().writeInt(minorVersion_);
        stream().writeInt(buildVersion_);
        stream().writeStringReference(fingerprintSha_);
        stream().writeInt(appStore_);
        stream().writeInt(deviceType_);
    }
    void decode() override {
        PiranhaMessage::decode();
        protocolVersion_ = stream().readInt();
        keyVersion_ = stream().readInt();
        majorVersion_ = stream().readInt();
        minorVersion_ = stream().readInt();
        buildVersion_ = stream().readInt();
        fingerprintSha_ = stream().readStringReference();
        appStore_ = stream().readInt();
        deviceType_ = stream().readInt();
    }
    i32 protocolVersion_ = 0, keyVersion_ = 0;
    i32 majorVersion_ = 0, minorVersion_ = 0, buildVersion_ = 0;
    std::string fingerprintSha_;
    i32 appStore_ = 0, deviceType_ = 0;
};

} // namespace titan
