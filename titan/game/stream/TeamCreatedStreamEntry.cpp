#pragma once

// TeamCreatedStreamEntry (alliance subtype 77) — reversed from libg_decrypted.so.
// Ctor @0x2b1d4c (size 0x48: base + fields below). Factory case 77 @0x90d928.
// encode @0x46381c, decode @0x765e90.
// Wire: StreamEntry base + nullable string +48 (-1 length form),
// vint +56, vint +60, vint +64.

#include "titan/game/stream/StreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class TeamCreatedStreamEntry : public StreamEntry {
public:
    int entryType() const { return 77; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        s.writeString(name48_ ? &*name48_ : nullptr);
        s.writeVInt(v56_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        name48_ = s.readString();
        v56_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
    }
    std::optional<std::string> name48_; // +48 (nullable)
    i32 v56_ = 0, v60_ = 0, v64_ = 0; // +56/+60/+64
};

} // namespace titan
