#pragma once

// DeviceLinkedStreamEntry (avatar subtype 9) — reversed from libg_decrypted.so.
// Ctor @0x86f4f4 adds no fields to the base. encode @0x5722f8 and
// decode @0x42c318 are both bare thunks into AvatarStreamEntry, so the
// wire format is exactly the base and the class carries no state.

#include "titan/game/stream/AvatarStreamEntry.cpp"

namespace titan {

class DeviceLinkedStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 9; }
};

} // namespace titan
