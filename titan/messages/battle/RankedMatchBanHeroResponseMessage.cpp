#pragma once

// RankedMatchBanHeroResponseMessage (22152) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchBanHeroResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22152; }
    const char* getMessageTypeName() const override { return "RankedMatchBanHeroResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(f132_);
        DataReference::encodeNullable(stream(), charRef_);
        stream().writeBoolean(f144_);
        stream().writeVInt(f148_);
    }
    void decode() override {
        PiranhaMessage::decode();
        f132_ = stream().readVInt();
        charRef_ = DataReference::decodeNullable(stream());
        f144_ = stream().readBoolean();
        f148_ = stream().readVInt();
    }
    i32 f132_ = 0, f148_ = 0;
    std::optional<DataReference> charRef_;
    bool f144_ = false;
};

} // namespace titan
