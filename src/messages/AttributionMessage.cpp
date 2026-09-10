// AttributionMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.

#include "titan/messages/AttributionMessage.hpp"

namespace titan {

void AttributionMessage::encode() {
    PiranhaMessage::encode();
    for (const auto& s : head_) stream().writeStringReference(s);
    stream().writeVInt(vintField_);
    stream().writeLongLong(longA_);
    stream().writeLongLong(longB_);
    for (bool f : flags_) stream().writeBoolean(f);
    for (const auto& s : tail_) stream().writeStringReference(s);
    stream().writeStringReference(last_);
}

void AttributionMessage::decode() {
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
