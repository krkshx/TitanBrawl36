#pragma once

// TeamStreamMessage (24131) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/StreamEntry.hpp"

namespace titan {

class TeamStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24131; }
    const char* getMessageTypeName() const override { return "TeamStreamMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0, v136_ = 0;
    std::vector<std::pair<i32, std::unique_ptr<StreamEntry>>> entries_;
    bool isNull_ = false;
};

} // namespace titan
