#pragma once

// TeamInvitation::encode @0x27b558.
// Wire: writeLong @+0, FriendEntry @+8 (required, direct call).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/core/LogicLong.cpp"
#include "titan/game/social/FriendEntry.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>

namespace titan {

class TeamInvitation : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id0_.encode(s); // == writeLong
        if (!friend_) throw pending_reverse("TeamInvitation needs FriendEntry");
        friend_->encode(s);
    }
    void decode(ByteStream& s) override {
        id0_ = LogicLong::decode(s);
        friend_ = std::make_unique<FriendEntry>();
        friend_->decode(s);
    }

    LogicLong id0_; // +0 (writeLong)
    std::unique_ptr<FriendEntry> friend_; // +8 (required)
};

} // namespace titan
