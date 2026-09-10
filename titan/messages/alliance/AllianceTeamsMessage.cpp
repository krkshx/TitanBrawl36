#pragma once

// AllianceTeamsMessage (24364) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/AllianceTeamEntry.hpp"

namespace titan {

class AllianceTeamsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24364; }
    const char* getMessageTypeName() const override { return "AllianceTeamsMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    std::vector<std::unique_ptr<AllianceTeamEntry>> teams_;
};

} // namespace titan

// AllianceTeamsMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceTeamsMessage::encode() {
PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    stream().writeVInt(static_cast<i32>(teams_.size()));
    for (const auto& t : teams_) t->encode(stream());
}

inline void AllianceTeamsMessage::decode() {
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
