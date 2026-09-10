#pragma once

// PlayerMapsMessage (22102) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/LogicPlayerMap.hpp"

namespace titan {

class PlayerMapsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22102; }
    const char* getMessageTypeName() const override { return "PlayerMapsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<LogicPlayerMap>> maps_;
    bool isNull_ = false;
};

} // namespace titan
