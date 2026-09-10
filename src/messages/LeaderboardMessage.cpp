// LeaderboardMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.

#include "titan/messages/LeaderboardMessage.hpp"

namespace titan {

void LeaderboardMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeVInt(v136_);
    DataReference::encodeNullable(stream(), ref_);
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeVInt(static_cast<i32>(rankings_.size()));
    for (const auto& r : rankings_) r->encode(stream());
    for (i32 x : tail_) stream().writeVInt(x);
    stream().writeString(tailStr_ ? &*tailStr_ : nullptr);
}

void LeaderboardMessage::decode() {
    PiranhaMessage::decode();
    v132_ = stream().readVInt();
    v136_ = stream().readVInt();
    ref_ = DataReference::decodeNullable(stream());
    name_ = stream().readString();
    const i32 n = stream().readVInt();
    rankings_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto r = std::make_unique<RankingEntry>();
        r->decode(stream());
        rankings_.push_back(std::move(r));
    }
    for (i32& x : tail_) x = stream().readVInt();
    tailStr_ = stream().readString();
}


// Wire: string id, bool hasId, [logiclong], string name, LogicClientAvatar.

} // namespace titan
