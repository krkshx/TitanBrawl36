#pragma once

// AllianceOnlineStatusUpdatedMessage (20207) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/StatusChangeEntry.hpp"

namespace titan {

class AllianceOnlineStatusUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20207; }
    const char* getMessageTypeName() const override { return "AllianceOnlineStatusUpdatedMessage"; }
    void encode() override;
    void decode() override;
    i32 statusType_ = 0;
    std::vector<std::unique_ptr<StatusChangeEntry>> changes_;
};

} // namespace titan
