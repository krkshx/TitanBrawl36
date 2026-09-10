#pragma once

// CreatePlayerMapResponseMessage (22100) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/LogicPlayerMap.hpp"

namespace titan {

class CreatePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22100; }
    const char* getMessageTypeName() const override { return "CreatePlayerMapResponseMessage"; }
    void encode() override;
    void decode() override;
    i32 resultCode_ = 0;
    bool hasMap_ = false;
    std::unique_ptr<LogicPlayerMap> map_;
};

} // namespace titan
