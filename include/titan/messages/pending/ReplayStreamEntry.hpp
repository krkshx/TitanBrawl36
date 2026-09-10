#pragma once

// ReplayStreamEntry — alliance-scope stream entry, not yet reversed.
// Derives StreamEntry so the alliance factory typechecks;
// encode/decode throw pending_reverse until reversed.

#include "titan/game/StreamEntry.hpp"

namespace titan {

class ReplayStreamEntry : public StreamEntry {
public:
    void encode(ByteStream&) const override {
        throw pending_reverse("ReplayStreamEntry pending reverse");
    }
    void decode(ByteStream&) override {
        throw pending_reverse("ReplayStreamEntry pending reverse");
    }
};

} // namespace titan
