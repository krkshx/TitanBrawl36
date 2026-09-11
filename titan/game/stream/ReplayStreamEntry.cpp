#pragma once

// ReplayStreamEntry (alliance subtype 5) — reversed from libg_decrypted.so.
// Ctor @0x8b6704 (size 0x70: base + fields below; +80 defaults to the
// template replay id from .rodata @0xba3ff4). Factory case 5 @0x90d964.
// encode @0x53f9b4, decode @0x7cc3ac.
// Wire: StreamEntry base, vint +44, LogicLong +48, bool +56,
// string +72, string +64 (in this order), string reference +80,
// vint +96, vint +100, vint +104. (Encoder slot map verified against
// the ChecksumEncoder vtable: +56 writeString, +128 writeVInt,
// +152 writeLong.)

#include "titan/core/LogicLong.cpp"
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
        long48_.encode(s);
        s.writeBoolean(b56_);
        s.writeString(text72_ ? &*text72_ : nullptr);
        s.writeString(text64_ ? &*text64_ : nullptr);
        s.writeStringReference(id80_);
        s.writeVInt(v96_);
        s.writeVInt(v100_);
        s.writeVInt(v104_);
    }
    void decode(ByteStream& s) override {
        StreamEntry::decode(s);
        v44_ = s.readVInt();
        long48_ = LogicLong::decode(s);
        b56_ = s.readBoolean();
        text72_ = s.readString();
        text64_ = s.readString();
        id80_ = s.readStringReference();
        v96_ = s.readVInt();
        v100_ = s.readVInt();
        v104_ = s.readVInt();
    }
    i32 v44_ = 0; // +44
    LogicLong long48_; // +48 (binary holds a pointer; port keeps a value)
    bool b56_ = false; // +56
    std::optional<std::string> text72_; // +72 (wired before +64, nullable)
    std::optional<std::string> text64_; // +64 (nullable)
    // +80: template replay id from the ctor (String::operator= from
    // .rodata @0xba3ff4).
    std::string id80_ = "44838203_a45f_46c9_9ec2_b0f70bb8a77f_12000";
    i32 v96_ = 0, v100_ = 0, v104_ = 0; // +96/+100/+104
};

} // namespace titan
