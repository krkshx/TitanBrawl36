// AllianceWarMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceWarMessage.hpp"

namespace titan {

void AllianceWarMessage::encode() {
    PiranhaMessage::encode();
    allianceId_.encode(stream());
    stream().writeVInt(field144_);
    stream().writeVInt(static_cast<i32>(nodes_.size()));
    for (const auto& x : nodes_) x->encode(stream());
    stream().writeVInt(static_cast<i32>(factions_.size()));
    for (const auto& x : factions_) x->encode(stream());
}

void AllianceWarMessage::decode() {
    PiranhaMessage::decode();
    allianceId_ = LogicLong::decode(stream());
    field144_ = stream().readVInt();
    const i32 nn = stream().readVInt();
    nodes_.clear();
    for (i32 i = 0; i < nn; ++i) {
        auto x = std::make_unique<AllianceWarNode>();
        x->decode(stream());
        nodes_.push_back(std::move(x));
    }
    const i32 nf = stream().readVInt();
    factions_.clear();
    for (i32 i = 0; i < nf; ++i) {
        auto x = std::make_unique<AllianceWarFaction>();
        x->decode(stream());
        factions_.push_back(std::move(x));
    }
}


} // namespace titan
