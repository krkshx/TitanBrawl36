#pragma once

// RankedMatchPickHeroFailedMessage (22155) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchPickHeroFailedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22155; }
    const char* getMessageTypeName() const override { return "RankedMatchPickHeroFailedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
        DataReference::encodeNullable(stream(), hero1_);
        stream().writeBoolean(flag_);
        DataReference::encodeNullable(stream(), hero2_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
        hero1_ = DataReference::decodeNullable(stream());
        flag_ = stream().readBoolean();
        hero2_ = DataReference::decodeNullable(stream());
    }
    i32 v_ = 0;
    std::optional<DataReference> hero1_, hero2_;
    bool flag_ = false;
};

} // namespace titan
