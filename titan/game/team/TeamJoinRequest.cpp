#pragma once

// TeamJoinRequest::encode @0x281300.
// Wire: writeLong @+0, writeLong @+8, FriendEntry @+16 (required).
// Decode is default-ctor + decode (see TeamEntry::decode @0x7c1a40):
// same reads in the same order (mirrored below).

#include "titan/core/LogicLong.cpp"
#include "titan/game/social/FriendEntry.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>

namespace titan {

class TeamJoinRequest : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id0_.encode(s); // == writeLong
        id8_.encode(s); // == writeLong
        if (!friend_) throw pending_reverse("TeamJoinRequest needs FriendEntry");
        friend_->encode(s);
    }
    void decode(ByteStream& s) override {
        id0_ = LogicLong::decode(s);
        id8_ = LogicLong::decode(s);
        friend_ = std::make_unique<FriendEntry>();
        friend_->decode(s);
    }

    LogicLong id0_, id8_;
    std::unique_ptr<FriendEntry> friend_; // +16 (required)
};

} // namespace titan
