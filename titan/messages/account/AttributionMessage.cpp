#pragma once

// AttributionMessage (30000) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

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

// AttributionMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.


namespace titan {

inline void AttributionMessage::encode() {
PiranhaMessage::encode();
    for (const auto& s : head_) stream().writeStringReference(s);
    stream().writeVInt(vintField_);
    stream().writeLongLong(longA_);
    stream().writeLongLong(longB_);
    for (bool f : flags_) stream().writeBoolean(f);
    for (const auto& s : tail_) stream().writeStringReference(s);
    stream().writeStringReference(last_);
}

inline void AttributionMessage::decode() {
PiranhaMessage::decode();
    for (auto& s : head_) s = stream().readStringReference();
    vintField_ = stream().readVInt();
    longA_ = stream().readLongLong();
    longB_ = stream().readLongLong();
    for (bool& f : flags_) f = stream().readBoolean();
    for (auto& s : tail_) s = stream().readStringReference();
    last_ = stream().readStringReference();
}


} // namespace titan
