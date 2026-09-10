#pragma once

// AllianceStreamEntryMessage (24312) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/StreamEntry.hpp"

namespace titan {

class AllianceStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24312; }
    const char* getMessageTypeName() const override { return "AllianceStreamEntryMessage"; }
    void encode() override;
    void decode() override;
    i32 entryType_ = 0;
    std::unique_ptr<StreamEntry> entry_;
};

} // namespace titan
