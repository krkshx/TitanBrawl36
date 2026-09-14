#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <string>

class ChangeAvatarNameMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10212; }
    std::int32_t getServiceNodeType() const override { return 9; }
    const char *getMessageTypeName() const override { return "ChangeAvatarNameMessage"; }

    void setName(const std::string &name) { name_ = name; }

    void encode() override {
        // По либе (_ZN23ChangeAvatarNameMessage6encodeEv @ 0x3ee5e8):
        // строка ника + bool nameSetByUser(true при ручном вводе).
        ByteStream *s = getByteStream();
        s->writeString(&name_);
        s->writeBoolean(true);
    }

    void decode() override {}

private:
    std::string name_;
};
