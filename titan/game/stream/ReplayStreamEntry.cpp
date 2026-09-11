#pragma once

// ReplayStreamEntry (alliance subtype 5) — reversed from libg_decrypted.so.
// Ctor @0x8b6704 (size 0x70: base + fields below; +80 defaults to the
// template replay id from .rodata @0xba3ff4). Factory case 5 @0x90d964.
// encode @0x53f9b4, decode @0x7cc3ac.
// Wire: StreamEntry base, vint +44, nullable string +48 (-1 length form),
// bool +56, vint +72, vint +64 (in this order), string reference +80,
// vint +96, vint +100, vint +104. The binary caps the +64/+72/+80 reads
// (900000); the port reads plain values like everywhere else.

#include "titan/game/stream/StreamEntry.cpp"

#include <optional>
#include <string>

namespace titan {

class ReplayStreamEntry : public StreamEntry {
public:
    int entryType() const { return 5; }
    void encode(ByteStream& s) const override {
        StreamEntry::encode(s);
        s.writeVInt(v44_);
        s.writeString(name48_ ? &*name48_ : nullptr);
        s.writeBoolean(b56_);
        s.writeVInt(v72_);
        s.writeVInt(v64_);
        s.writeStringReference(id80_);
        s.writeVInt(v96_);
        s.writeVInt(v100_);
        s.writeVInt(v104_);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        v44_ = s.readVInt();
        name48_ = s.readString();
        b56_ = s.readBoolean();
        v72_ = s.readVInt();
        v64_ = s.readVInt();
        id80_ = s.readStringReference();
        v96_ = s.readVInt();
        v100_ = s.readVInt();
        v104_ = s.readVInt();
    }
    i32 v44_ = 0; // +44
    std::optional<std::string> name48_; // +48 (nullable)
    bool b56_ = false; // +56
    i32 v72_ = 0; // +72 (wired before +64)
    i32 v64_ = 0; // +64
    // +80: template replay id from the ctor (String::operator= from
    // .rodata @0xba3ff4).
    std::string id80_ = "44838203_a45f_46c9_9ec2_b0f70bb8a77f_12000";
    i32 v96_ = 0, v100_ = 0, v104_ = 0; // +96/+100/+104
};

} // namespace titan
