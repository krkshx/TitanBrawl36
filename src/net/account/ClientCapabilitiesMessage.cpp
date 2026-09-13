#pragma once
#include "../core/PiranhaMessage.cpp"

class ClientCapabilitiesMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10107; }
    std::int32_t getServiceNodeType() const override { return 1; }
    void encode() override {
        getByteStream()->writeVInt(ping_);
    }
    void decode() override {
        ping_ = getByteStream()->readVInt();
    }
    void setPing(std::int32_t v) { ping_ = v; }
    std::int32_t ping() const { return ping_; }
private:
    std::int32_t ping_ = 0;
};
