// TencentAntiAddictionInstructionMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.

#include "titan/messages/TencentAntiAddictionInstructionMessage.hpp"

namespace titan {

void TencentAntiAddictionInstructionMessage::encode() {
    PiranhaMessage::encode();
    stream().writeStringReference(name_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(instructions_.size()));
    for (const auto& x : instructions_) x->encode(stream());
}

void TencentAntiAddictionInstructionMessage::decode() {
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
