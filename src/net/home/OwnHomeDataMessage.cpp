#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <vector>

class OwnHomeDataMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 24101; }
    std::int32_t getServiceNodeType() const override { return 9; }
    const char *getMessageTypeName() const override { return "OwnHomeDataMessage"; }

    void encode() override {}

    void decode() override {
        ByteStream *s = getByteStream();
        const char *base = s->getByteArray();
        std::int32_t n = s->getLength();
        raw_.assign(base, base + n);
    }

    const std::vector<char> &raw() const { return raw_; }

private:
    std::vector<char> raw_;
};
