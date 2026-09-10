#pragma once

// AvatarStreamEntryMessage (24412) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/AvatarStreamEntry.hpp"

namespace titan {

class AvatarStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24412; }
    const char* getMessageTypeName() const override { return "AvatarStreamEntryMessage"; }
    void encode() override;
    void decode() override;
    i32 entryType_ = 0;
    std::unique_ptr<AvatarStreamEntry> entry_;
};

} // namespace titan
