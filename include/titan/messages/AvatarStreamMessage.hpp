#pragma once

// AvatarStreamMessage (24411) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/AvatarStreamEntry.hpp"

namespace titan {

class AvatarStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24411; }
    const char* getMessageTypeName() const override { return "AvatarStreamMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::pair<i32, std::unique_ptr<AvatarStreamEntry>>> entries_;
    bool isNull_ = false;
};

} // namespace titan
