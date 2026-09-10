#pragma once

// MatchmakeRequestMessage (14103) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class MatchmakeRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14103; }
    const char* getMessageTypeName() const override { return "MatchmakeRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), eventRef_);
        for (i32 x : vints_) stream().writeVInt(x);
    }
    void decode() override {
        PiranhaMessage::decode();
        eventRef_ = DataReference::decodeNullable(stream());
        for (i32& x : vints_) x = stream().readVInt();
    }
    std::optional<DataReference> eventRef_;
    i32 vints_[4] = {};
};

} // namespace titan
