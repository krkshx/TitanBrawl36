#pragma once

// StartLoadingMessage (20559) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/messages/pending/BattlePlayerMap.cpp"
#include "titan/game/LogicPlayer.hpp"
#include "titan/game/LogicVector2.hpp"

namespace titan {

class StartLoadingMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20559; }
    const char* getMessageTypeName() const override { return "StartLoadingMessage"; }
    void encode() override;
    void decode() override;
    i32 head_[3] = {}; // unk132..140
    std::vector<std::unique_ptr<LogicPlayer>> players_;   // int count
    std::vector<std::unique_ptr<LogicVector2>> spawns_;   // int count
    std::vector<i32> unkInts_;                            // int count
    i32 unk144_ = 0;
    i32 vints_[5] = {}; // unk148..164 (vint)
    // NOTE: spec order lists vint unk168 between unk164 and mapRef.
    i32 v168_ = 0;
    std::optional<DataReference> mapRef_;
    std::unique_ptr<BattlePlayerMap> battleMap_;
    bool unk172_ = false, unk173_ = false;
    i32 v176_ = 0, v180_ = 0;
};

} // namespace titan

// StartLoadingMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.


namespace titan {

inline void StartLoadingMessage::encode() {
PiranhaMessage::encode();
    for (i32 x : head_) stream().writeInt(x);
    stream().writeInt(static_cast<i32>(players_.size()));
    for (const auto& p : players_) p->encode(stream());
    stream().writeInt(static_cast<i32>(spawns_.size()));
    for (const auto& s : spawns_) s->encode(stream());
    stream().writeInt(static_cast<i32>(unkInts_.size()));
    for (i32 x : unkInts_) stream().writeInt(x);
    stream().writeInt(unk144_);
    for (i32 x : vints_) stream().writeVInt(x);
    stream().writeVInt(v168_);
DataReference::encodeNullable(stream(), mapRef_);
    if (!battleMap_) throw pending_reverse("StartLoadingMessage needs BattlePlayerMap");
    battleMap_->encode(stream());
    stream().writeBoolean(unk172_);
    stream().writeBoolean(unk173_);
    stream().writeVInt(v176_);
    stream().writeVInt(v180_);
}

inline void StartLoadingMessage::decode() {
PiranhaMessage::decode();
    for (i32& x : head_) x = stream().readInt();
    const i32 np = stream().readInt();
    players_.clear();
    for (i32 i = 0; i < np; ++i) {
        auto p = std::make_unique<LogicPlayer>();
        p->decode(stream());
        players_.push_back(std::move(p));
    }
    const i32 ns = stream().readInt();
    spawns_.clear();
    for (i32 i = 0; i < ns; ++i) {
        auto s = std::make_unique<LogicVector2>();
        s->decode(stream());
        spawns_.push_back(std::move(s));
    }
    const i32 ni = stream().readInt();
    unkInts_.clear();
    for (i32 i = 0; i < ni; ++i) unkInts_.push_back(stream().readInt());
    unk144_ = stream().readInt();
    for (i32& x : vints_) x = stream().readVInt();
    v168_ = stream().readVInt();
    mapRef_ = DataReference::decodeNullable(stream());
    battleMap_ = std::make_unique<BattlePlayerMap>();
    battleMap_->decode(stream());
    unk172_ = stream().readBoolean();
    unk173_ = stream().readBoolean();
    v176_ = stream().readVInt();
    v180_ = stream().readVInt();
}


} // namespace titan
