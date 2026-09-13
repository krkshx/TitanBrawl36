#pragma once
#include "PiranhaMessage.cpp"
#include <cstdint>
#include <vector>

// Дженерик для неизвестных/неразобранных типов: хранит сырой payload,
// насос сообщений не умирает. FriendListMessage = 20105.
class GenericMessage : public PiranhaMessage {
public:
    explicit GenericMessage(std::int32_t type) : type_(type) {}
    std::int32_t getMessageType() const override { return type_; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "GenericMessage"; }
    void encode() override {}
    void decode() override {
        ByteStream *s = getByteStream();
        const char *base = s->getByteArray();
        std::int32_t n = s->getLength();
        raw_.assign(base, base + n);
    }
    const std::vector<char> &raw() const { return raw_; }
private:
    std::int32_t type_;
    std::vector<char> raw_;
};
