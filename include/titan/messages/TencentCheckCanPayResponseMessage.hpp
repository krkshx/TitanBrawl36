#pragma once

// TencentCheckCanPayResponseMessage (24223) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
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
