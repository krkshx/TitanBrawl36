#pragma once

// TencentAntiAddictionInstructionMessage (24222) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
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
