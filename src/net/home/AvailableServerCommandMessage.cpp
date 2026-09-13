#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <vector>

// AvailableServerCommandMessage (24111, node 9): VInt commandType + тело команды.
// Полный разбор команд — позже; пока тип + сырые байты, чтобы не рвать цепочку.
class AvailableServerCommandMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 24111; }
    std::int32_t getServiceNodeType() const override { return 9; }
    const char *getMessageTypeName() const override { return "AvailableServerCommandMessage"; }
    void encode() override {}
    void decode() override {
        ByteStream *s = getByteStream();
        if (!s->isAtEnd()) {
            commandType_ = s->readVInt();
        }
        const char *base = s->getByteArray();
        std::int32_t n = s->getLength();
        std::int32_t off = s->getOffset();
        if (off < n) {
            raw_.assign(base + off, base + n);
        }
    }
    std::int32_t commandType() const { return commandType_; }
    const std::vector<char> &raw() const { return raw_; }
private:
    std::int32_t commandType_ = 0;
    std::vector<char> raw_;
};
