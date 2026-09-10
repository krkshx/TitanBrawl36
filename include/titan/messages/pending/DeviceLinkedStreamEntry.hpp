#pragma once

// DeviceLinkedStreamEntry — avatar-scope stream entry, not yet reversed.
// Derives AvatarStreamEntry so the avatar factory typechecks;
// encode/decode throw pending_reverse until reversed.

#include "titan/game/AvatarStreamEntry.hpp"

namespace titan {

class DeviceLinkedStreamEntry : public AvatarStreamEntry {
public:
    void encode(ByteStream&) const override {
        throw pending_reverse("DeviceLinkedStreamEntry pending reverse");
    }
    void decode(ByteStream&) override {
        throw pending_reverse("DeviceLinkedStreamEntry pending reverse");
    }
};

} // namespace titan
