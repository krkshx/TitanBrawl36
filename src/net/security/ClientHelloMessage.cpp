#pragma once
#include "../core/PiranhaMessage.cpp"
#include <string>

class ClientHelloMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10100; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "ClientHelloMessage"; }

    void encode() override {
        ByteStream *s = getByteStream();
        s->writeInt(field0_);
        s->writeInt(field1_);
        s->writeInt(field2_);
        s->writeInt(clientSeed_);
        s->writeInt(field4_);
        s->writeStringReference(contentHash_);
        s->writeInt(field5_);
        s->writeInt(field6_);
    }

    void decode() override {
        ByteStream *s = getByteStream();
        field0_ = s->readInt();
        field1_ = s->readInt();
        field2_ = s->readInt();
        clientSeed_ = s->readInt();
        field4_ = s->readInt();
        contentHash_ = s->readStringReference(900000);
        field5_ = s->readInt();
        field6_ = s->readInt();
    }

    void setClientSeed(std::int32_t v) { clientSeed_ = v; }
    void setContentHash(const std::string &v) { contentHash_ = v; }

private:
    std::int32_t field0_ = 0;
    std::int32_t field1_ = 0;
    std::int32_t field2_ = 0;
    std::int32_t clientSeed_ = 0;
    std::int32_t field4_ = 0;
    std::string contentHash_;
    std::int32_t field5_ = 0;
    std::int32_t field6_ = 0;
};
