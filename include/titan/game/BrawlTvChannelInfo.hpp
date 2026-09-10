#pragma once

// BrawlTvChannelInfo::encode @0x871e14 (const).
// Wire: vint @+0, string (nullable String* @+8/@+16).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

class BrawlTvChannelInfo : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeString(s8_.has_value() ? &s8_.value() : nullptr);
        s.writeString(s16_.has_value() ? &s16_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        s8_ = s.readString();
        s16_ = s.readString();
    }

    i32 v0_ = 0;
    std::optional<std::string> s8_, s16_;
};

} // namespace titan
