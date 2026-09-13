#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>

class ResetAccountMessage : public PiranhaMessage {
public:
    std::int32_t id() const override { return 10116; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(preset_);
    }
    void decode() override {
        preset_ = stream().readInt();
    }
    void setPreset(std::int32_t v) { preset_ = v; }
    std::int32_t preset() const { return preset_; }
private:
    std::int32_t preset_ = 0;
};
