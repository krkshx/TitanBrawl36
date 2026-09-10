#pragma once

// TeamInviteEntry::encode @0x3900dc.
// Wire: writeLong @+0, writeLong @+8, string (nullable @+16),
//   vint @+24/+28.
// Decode is default-ctor + decode (see TeamEntry::decode @0x7c1a40):
// same reads in the same order (mirrored below).

#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

class TeamInviteEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id0_.encode(s); // == writeLong
        id8_.encode(s); // == writeLong
        s.writeString(name_.has_value() ? &name_.value() : nullptr);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
    }
    void decode(ByteStream& s) override {
        id0_ = LogicLong::decode(s);
        id8_ = LogicLong::decode(s);
        name_ = s.readString();
        v24_ = s.readVInt();
        v28_ = s.readVInt();
    }

    LogicLong id0_, id8_;
    std::optional<std::string> name_; // +16
    i32 v24_ = 0, v28_ = 0;
};

} // namespace titan
