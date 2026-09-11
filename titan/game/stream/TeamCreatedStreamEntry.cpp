#pragma once

// TeamCreatedStreamEntry (alliance subtype 77) — reversed from libg_decrypted.so.
// Ctor @0x2b1d4c (size 0x48: base + fields below). Factory case 77 @0x90d928.
// encode @0x46381c, decode @0x765e90.
// Wire: StreamEntry base + LogicLong +48, vint +56, vint +60, vint +64.
// (Encoder slot map verified: +152 writeLong, +128 writeVInt.)

#include "titan/core/LogicLong.cpp"
#include "titan/game/stream/StreamEntry.cpp"

namespace titan {

class TeamCreatedStreamEntry : public StreamEntry {
public:
    int entryType() const { return 77; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        long48_.encode(s);
        s.writeVInt(v56_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        long48_ = LogicLong::decode(s);
        v56_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
    }
    LogicLong long48_; // +48 (binary holds a pointer; port keeps a value)
    i32 v56_ = 0, v60_ = 0, v64_ = 0; // +56/+60/+64
};

} // namespace titan
