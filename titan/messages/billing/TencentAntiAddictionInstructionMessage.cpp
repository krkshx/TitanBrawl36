#pragma once

// TencentAntiAddictionInstructionMessage (24222) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/LogicTencentAntiAddictionInstruction.hpp"

namespace titan {

class TencentAntiAddictionInstructionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24222; }
    const char* getMessageTypeName() const override { return "TencentAntiAddictionInstructionMessage"; }
    void encode() override;
    void decode() override;
    std::string name_;
    std::vector<std::unique_ptr<LogicTencentAntiAddictionInstruction>> instructions_;
    bool isNull_ = false;
};

} // namespace titan

// TencentAntiAddictionInstructionMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.


namespace titan {

inline void TencentAntiAddictionInstructionMessage::encode() {
PiranhaMessage::encode();
    stream().writeStringReference(name_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(instructions_.size()));
    for (const auto& x : instructions_) x->encode(stream());
}

inline void TencentAntiAddictionInstructionMessage::decode() {
PiranhaMessage::decode();
    name_ = stream().readStringReference();
    const i32 n = stream().readVInt();
    instructions_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        auto x = std::make_unique<LogicTencentAntiAddictionInstruction>();
        x->decode(stream());
        instructions_.push_back(std::move(x));
    }
}

} // namespace titan
