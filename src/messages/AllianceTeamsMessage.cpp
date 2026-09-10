// AllianceTeamsMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceTeamsMessage.hpp"

namespace titan {

void AllianceTeamsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    stream().writeVInt(static_cast<i32>(teams_.size()));
    for (const auto& t : teams_) t->encode(stream());
}

void AllianceTeamsMessage::decode() {
    PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    const i32 n = stream().readVInt();
    teams_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto t = std::make_unique<AllianceTeamEntry>();
        t->decode(stream());
        teams_.push_back(std::move(t));
    }
}


} // namespace titan
