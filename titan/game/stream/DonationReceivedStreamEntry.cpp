#pragma once

// DonationReceivedStreamEntry (avatar subtype 7) — reversed from libg_decrypted.so.
// Ctor @0x26d4d4 (size 0x30: base + one vint below).
// encode @0x44a298, decode @0x31fd18.
// Wire: AvatarStreamEntry base + vint +44.

#include "titan/game/stream/AvatarStreamEntry.cpp"

namespace titan {

class DonationReceivedStreamEntry : public AvatarStreamEntry {
public:
    int entryType() const { return 7; }
    void encode(ByteStream& s) const override {
        AvatarStreamEntry::encode(s);
        s.writeVInt(v44_);
    }
    void decode(ByteStream& s) override {
        AvatarStreamEntry::decode(s);
        v44_ = s.readVInt();
    }
    i32 v44_ = 0; // +44
};

} // namespace titan
