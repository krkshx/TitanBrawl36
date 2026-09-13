#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <vector>

class ServerHelloMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 20100; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "ServerHelloMessage"; }

    void encode() override {
        ByteStream *s = getByteStream();
        s->writeBytes(reinterpret_cast<const char *>(token_.data()), static_cast<std::int32_t>(token_.size()));
    }

    void decode() override {
        ByteStream *s = getByteStream();
        std::int32_t n = s->readBytesLength();
        token_ = s->readBytes(n, 1000);
    }

    const std::vector<std::uint8_t> &token() const { return token_; }

private:
    std::vector<std::uint8_t> token_;
};
