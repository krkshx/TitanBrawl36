// AskForBattleEndMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.

#include "titan/messages/AskForBattleEndMessage.hpp"

namespace titan {

void AskForBattleEndMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v1_);
    stream().writeVInt(v2_);
    stream().writeVInt(v3_);
    DataReference::encodeNullable(stream(), location_);
    stream().writeVInt(static_cast<i32>(heroes_.size()));
    for (const auto& h : heroes_) h->encode(stream());
}

void AskForBattleEndMessage::decode() {
    PiranhaMessage::decode();
    v1_ = stream().readVInt();
    v2_ = stream().readVInt();
    v3_ = stream().readVInt();
    location_ = DataReference::decodeNullable(stream());
    const i32 n = stream().readVInt();
    heroes_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto h = std::make_unique<HeroDataEntry>();
        h->decode(stream());
        heroes_.push_back(std::move(h));
    }
}


} // namespace titan
