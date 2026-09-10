#pragma once

// TeamCreateMessage (14350) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamCreateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14350; }
    const char* getMessageTypeName() const override { return "TeamCreateMessage"; }
    void encode() override;
    void decode() override;
    i32 v136_ = 0, v140_ = 0, teamType_ = 0;
    bool unk144_ = false, hasInvite_ = false;
    LogicLong inviteId_;
    i32 unk160_ = 0;
    std::optional<DataReference> eventRef_;
    LogicLong roomId_;
};

} // namespace titan
