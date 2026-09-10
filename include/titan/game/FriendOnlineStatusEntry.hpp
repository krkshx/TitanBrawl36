#pragma once

// FriendOnlineStatusEntry::encode @0x1a39e8, decode @0x26a79c.
// Wire: writeLong @+0, vint +16/+20, bool +24,
//   bool + AllianceTeamEntry @+8 (nullable).
// (The pending stub guessed the name FriendOnlineStatus; the binary's
// class is FriendOnlineStatusEntry — the stub is now an alias.)

#include "titan/core/LogicLong.hpp"
#include "titan/game/AllianceTeamEntry.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>

namespace titan {

class FriendOnlineStatusEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id0_.encode(s); // == writeLong
        s.writeVInt(v16_);
        s.writeVInt(v20_);
        s.writeBoolean(b24_);
        s.writeBoolean(team_ != nullptr);
        if (team_) team_->encode(s);
    }
    void decode(ByteStream& s) override {
        id0_ = LogicLong::decode(s);
        v16_ = s.readVInt();
        v20_ = s.readVInt();
        b24_ = s.readBoolean();
        if (s.readBoolean()) {
            team_ = std::make_unique<AllianceTeamEntry>();
            team_->decode(s);
        } else {
            team_.reset();
        }
    }

    LogicLong id0_; // +0 (writeLong)
    i32 v16_ = 0, v20_ = 0;
    bool b24_ = false;
    std::unique_ptr<AllianceTeamEntry> team_; // +8 (nullable)
};

} // namespace titan
