#pragma once

// AskForBattleEndMessage (14110) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/avatar/HeroDataEntry.cpp"

namespace titan {

class AskForBattleEndMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14110; }
    const char* getMessageTypeName() const override { return "AskForBattleEndMessage"; }
    void encode() override;
    void decode() override;
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
    std::optional<DataReference> location_;
    std::vector<std::unique_ptr<HeroDataEntry>> heroes_;
};

} // namespace titan

// AskForBattleEndMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.


namespace titan {

inline void AskForBattleEndMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(v1_);
    stream().writeVInt(v2_);
    stream().writeVInt(v3_);
DataReference::encodeNullable(stream(), location_);
    stream().writeVInt(static_cast<i32>(heroes_.size()));
    for (const auto& h : heroes_) h->encode(stream());
}

inline void AskForBattleEndMessage::decode() {
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
