#pragma once

// TencentAntiAddictionInstructionExecutedMessage (14267) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

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
