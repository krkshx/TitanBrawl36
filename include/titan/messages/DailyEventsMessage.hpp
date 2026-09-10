#pragma once

// DailyEventsMessage (24123) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/EventData.hpp"

namespace titan {

class DailyEventsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24123; }
    const char* getMessageTypeName() const override { return "DailyEventsMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0, f136_ = 0;
    std::vector<std::unique_ptr<EventData>> events_;
};

} // namespace titan
