#pragma once

// RankedMatchUpdateHeroDataMessage (12157) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class RankedMatchUpdateHeroDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12157; }
    const char* getMessageTypeName() const override { return "RankedMatchUpdateHeroDataMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), ref1_);
        DataReference::encodeNullable(stream(), ref2_);
        DataReference::encodeNullable(stream(), ref3_);
    }
    void decode() override {
        PiranhaMessage::decode();
        ref1_ = DataReference::decodeNullable(stream());
        ref2_ = DataReference::decodeNullable(stream());
        ref3_ = DataReference::decodeNullable(stream());
    }
    std::optional<DataReference> ref1_, ref2_, ref3_;
};

} // namespace titan
