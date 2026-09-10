// TencentCheckCanPayResponseMessage bodies — split out of MsgBatch12.cpp; wire format unchanged.

#include "titan/messages/TencentCheckCanPayResponseMessage.hpp"

namespace titan {

void TencentCheckCanPayResponseMessage::encode() {
    PiranhaMessage::encode();
    stream().writeStringReference(data_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(instructions_.size()));
    for (const auto& x : instructions_) x->encode(stream());
}

void TencentCheckCanPayResponseMessage::decode() {
    PiranhaMessage::decode();
    data_ = stream().readStringReference();
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
