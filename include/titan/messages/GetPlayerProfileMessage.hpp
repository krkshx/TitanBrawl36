#pragma once

// GetPlayerProfileMessage (14113) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/BattleLogPlayerEntry.hpp"

namespace titan {

class GetPlayerProfileMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14113; }
    const char* getMessageTypeName() const override { return "GetPlayerProfileMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    bool hasEntry_ = false;
    std::unique_ptr<BattleLogPlayerEntry> entry_;
    i32 unknown_ = 0;
};

} // namespace titan
