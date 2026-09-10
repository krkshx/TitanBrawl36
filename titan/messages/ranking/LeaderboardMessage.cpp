#pragma once

// LeaderboardMessage (24403) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/RankingEntry.hpp"

namespace titan {

class LeaderboardMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24403; }
    const char* getMessageTypeName() const override { return "LeaderboardMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0, v136_ = 0;
    std::optional<DataReference> ref_;
    std::optional<std::string> name_;
    std::vector<std::unique_ptr<RankingEntry>> rankings_;
    i32 tail_[4] = {}; // v168..v180
    std::optional<std::string> tailStr_;
};

} // namespace titan

// LeaderboardMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.


namespace titan {

inline void LeaderboardMessage::encode() {
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

inline void LeaderboardMessage::decode() {
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
