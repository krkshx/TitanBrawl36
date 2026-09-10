#pragma once

// TeamCreatedStreamEntry — alliance-scope stream entry, not yet reversed.
// Derives StreamEntry so the alliance factory typechecks;
// encode/decode throw pending_reverse until reversed.

#include "titan/game/stream/StreamEntry.cpp"

namespace titan {

class TeamCreatedStreamEntry : public StreamEntry {
public:
    void encode(ByteStream&) const override {
        throw pending_reverse("TeamCreatedStreamEntry pending reverse");
    }
    void decode(ByteStream&) override {
        throw pending_reverse("TeamCreatedStreamEntry pending reverse");
    }
};

} // namespace titan
