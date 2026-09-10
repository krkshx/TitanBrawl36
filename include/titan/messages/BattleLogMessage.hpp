#pragma once

// BattleLogMessage (23458) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/BattleLogEntry.hpp"

namespace titan {

class BattleLogMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23458; }
    const char* getMessageTypeName() const override { return "BattleLogMessage"; }
    void encode() override;
    void decode() override;
    bool b132_ = false;
    std::vector<std::unique_ptr<BattleLogEntry>> entries_;
};

} // namespace titan
