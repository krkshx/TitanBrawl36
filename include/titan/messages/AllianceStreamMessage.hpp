#pragma once

// AllianceStreamMessage (24311) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/StreamEntry.hpp"

namespace titan {

class AllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24311; }
    const char* getMessageTypeName() const override { return "AllianceStreamMessage"; }
    void encode() override;
    void decode() override;
    // (entryType, entry) pairs; empty vector encodes count -1? No:
    // count -1 means null; we always write the real count.
    std::vector<std::pair<i32, std::unique_ptr<StreamEntry>>> entries_;
    bool isNull_ = false; // -> writeVInt(-1)
};

} // namespace titan
