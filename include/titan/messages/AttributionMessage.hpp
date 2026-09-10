#pragma once

// AttributionMessage (30000) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AttributionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 30000; }
    const char* getMessageTypeName() const override { return "AttributionMessage"; }
    void encode() override;
    void decode() override;
    std::string head_[11];   // s_136..s_312
    i32 vintField_ = 0;      // field_612
    i64 longA_ = 0, longB_ = 0; // field_600/592 via writeLongLong
    bool flags_[4] = {};     // 616, 608, 617, 328
    std::string tail_[16];   // s_336..s_576
    std::string last_;       // s_216
};

} // namespace titan
