#pragma once

// EndClientTurnMessage (14102) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/commands/LogicCommand.hpp"
#include "titan/gen/LogicCommands.hpp"

namespace titan {

class EndClientTurnMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14102; }
    const char* getMessageTypeName() const override { return "EndClientTurnMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    i32 tick_ = 0, checksum_ = 0;
    std::vector<std::unique_ptr<LogicCommand>> commands_;
    std::optional<std::vector<u8>> payload_; // bytes, -1=null
};

} // namespace titan
