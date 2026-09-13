#pragma once
#include "../core/PiranhaMessage.cpp"

class TitanDisconnectedMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 25892; }
    std::int32_t getServiceNodeType() const override { return 1; }
    void encode() override {
        getByteStream()->writeInt(reason_);
    }
    void decode() override {
        reason_ = getByteStream()->readInt();
    }
    void setReason(std::int32_t v) { reason_ = v; }
    std::int32_t reason() const { return reason_; }
private:
    std::int32_t reason_ = 0;
};
