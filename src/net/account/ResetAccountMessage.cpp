#pragma once
#include "../core/PiranhaMessage.cpp"

class ResetAccountMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 10116; }
    std::int32_t getServiceNodeType() const override { return 9; }
    void encode() override {
        getByteStream()->writeInt(preset_);
    }
    void decode() override {
        preset_ = getByteStream()->readInt();
    }
    void setPreset(std::int32_t v) { preset_ = v; }
    std::int32_t preset() const { return preset_; }
private:
    std::int32_t preset_ = 0;
};
