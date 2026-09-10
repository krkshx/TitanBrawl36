#pragma once

// TencentCheckCanPayResponseMessage (24223) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/LogicTencentAntiAddictionInstruction.hpp"

namespace titan {

class TencentCheckCanPayResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24223; }
    const char* getMessageTypeName() const override { return "TencentCheckCanPayResponseMessage"; }
    void encode() override;
    void decode() override;
    std::string data_;
    std::vector<std::unique_ptr<LogicTencentAntiAddictionInstruction>> instructions_;
    bool isNull_ = false;
};

} // namespace titan

// TencentCheckCanPayResponseMessage bodies — split out of MsgBatch12.cpp; wire format unchanged.


namespace titan {

inline void TencentCheckCanPayResponseMessage::encode() {
PiranhaMessage::encode();
    stream().writeStringReference(data_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(instructions_.size()));
    for (const auto& x : instructions_) x->encode(stream());
}

inline void TencentCheckCanPayResponseMessage::decode() {
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
