#pragma once

// TencentAntiAddictionInstructionExecutedMessage (14267) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TencentAntiAddictionInstructionExecutedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14267; }
    const char* getMessageTypeName() const override { return "TencentAntiAddictionInstructionExecutedMessage"; }
    void encode() override;
    void decode() override; // decode caps args at 11
    std::string name_;
    std::vector<std::string> args_;
    bool isNull_ = false;
};

} // namespace titan

// TencentAntiAddictionInstructionExecutedMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.


namespace titan {

inline void TencentAntiAddictionInstructionExecutedMessage::encode() {
PiranhaMessage::encode();
    stream().writeStringReference(name_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(args_.size()));
    for (const auto& a : args_) stream().writeString(&a);
}

inline void TencentAntiAddictionInstructionExecutedMessage::decode() {
PiranhaMessage::decode();
    name_ = stream().readStringReference();
    const i32 n = stream().readVInt();
    args_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n && i < 11; ++i) args_.push_back(stream().readString().value_or(""));
}


} // namespace titan
