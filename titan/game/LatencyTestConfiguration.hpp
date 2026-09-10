#pragma once

// LatencyTestConfiguration::encode @0x3ffdc4.
// Wire: vint +0/+4/+8/+12/+16/+20/+24, bool +28/+29, bool +48 (wired
// before +32!), vint +32, string (nullable @+40),
//   writeBytes @+64 (len +72), writeBytes @+80 (len +88),
//   then iff b48: vint +52 + string (nullable @+56).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.cpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LatencyTestConfiguration : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : head_) s.writeVInt(v);
        s.writeBoolean(b28_);
        s.writeBoolean(b29_);
        s.writeBoolean(b48_);
        s.writeVInt(v32_);
        s.writeString(s40_.has_value() ? &s40_.value() : nullptr);
        // NB: writeBytes(null, ·) wires -1, so empty vectors need a
        // non-null pointer to wire length 0 like the binary.
        static const u8 kEmpty = 0;
        s.writeBytes(bytes64_.empty() ? &kEmpty : bytes64_.data(),
                     static_cast<i32>(bytes64_.size()));
        s.writeBytes(bytes80_.empty() ? &kEmpty : bytes80_.data(),
                     static_cast<i32>(bytes80_.size()));
        if (b48_) {
            s.writeVInt(v52_);
            s.writeString(s56_.has_value() ? &s56_.value() : nullptr);
        }
    }
    void decode(ByteStream& s) override {
        for (i32& v : head_) v = s.readVInt();
        b28_ = s.readBoolean();
        b29_ = s.readBoolean();
        b48_ = s.readBoolean();
        v32_ = s.readVInt();
        s40_ = s.readString();
        bytes64_ = s.readBytes();
        bytes80_ = s.readBytes();
        if (b48_) {
            v52_ = s.readVInt();
            s56_ = s.readString();
        }
    }

    i32 head_[7] = {}; // +0..+24
    bool b28_ = false, b29_ = false;
    bool b48_ = false; // +48 (wired early)
    i32 v32_ = 0;
    std::optional<std::string> s40_; // +40
    std::vector<u8> bytes64_, bytes80_; // +64/+80 (writeBytes)
    i32 v52_ = 0; // present iff b48
    std::optional<std::string> s56_; // present iff b48
};

} // namespace titan
