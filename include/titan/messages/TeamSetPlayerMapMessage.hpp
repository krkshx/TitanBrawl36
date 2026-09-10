#pragma once

// TeamSetPlayerMapMessage (12110) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamSetPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12110; }
    const char* getMessageTypeName() const override { return "TeamSetPlayerMapMessage"; }
    void encode() override;
    void decode() override;
    LogicLong mapId_;
    std::vector<i32> intList_; // vint count + vint items (verify on capture)
};

} // namespace titan
