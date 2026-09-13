#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <string>

class CreateAccountOkMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 26007; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "CreateAccountOkMessage"; }

    void encode() override {}

    void decode() override {
        ByteStream *s = getByteStream();
        passToken_ = s->readStringReference(900000);
        LogicLong id;
        s->readLong(&id);
        accountId_ = (static_cast<std::int64_t>(id.high) << 32) | static_cast<std::uint32_t>(id.low);
    }

    std::int64_t accountId() const { return accountId_; }
    const std::string &passToken() const { return passToken_; }

private:
    std::string passToken_;
    std::int64_t accountId_ = 0;
};
