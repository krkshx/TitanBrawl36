#pragma once

// JoinAllianceResponseAvatarStreamEntry — avatar-scope stream entry, not yet reversed.
// Derives AvatarStreamEntry so the avatar factory typechecks;
// encode/decode throw pending_reverse until reversed.

#include "titan/game/stream/AvatarStreamEntry.cpp"

namespace titan {

class JoinAllianceResponseAvatarStreamEntry : public AvatarStreamEntry {
public:
    void encode(ByteStream&) const override {
        throw pending_reverse("JoinAllianceResponseAvatarStreamEntry pending reverse");
    }
    void decode(ByteStream&) override {
        throw pending_reverse("JoinAllianceResponseAvatarStreamEntry pending reverse");
    }
};

} // namespace titan
