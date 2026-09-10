#pragma once

// UpdatePlayerMapMessage (12103) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class UpdatePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12103; }
    const char* getMessageTypeName() const override { return "UpdatePlayerMapMessage"; }
    void encode() override;
    void decode() override;
    LogicLong mapId_;
    std::unique_ptr<LogicCompressedString> mapData_;
};

} // namespace titan
