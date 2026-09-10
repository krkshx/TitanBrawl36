#pragma once

// RankedMatchBanHeroMessage (12152) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchBanHeroMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12152; }
    const char* getMessageTypeName() const override { return "RankedMatchBanHeroMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), charRef_);
        stream().writeVInt(slot_);
    }
    void decode() override {
        PiranhaMessage::decode();
        charRef_ = DataReference::decodeNullable(stream());
        hasSlot_ = !stream().isAtEnd();
        if (hasSlot_) slot_ = stream().readVInt();
    }
    std::optional<DataReference> charRef_;
    i32 slot_ = 0;
    bool hasSlot_ = false;
};

} // namespace titan
