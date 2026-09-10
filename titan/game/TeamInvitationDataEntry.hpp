#pragma once

// TeamInvitationDataEntry::encode @0x65bdf8.
// Wire: writeLong @+0, string (nullable String* @+8).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.cpp"

#include <optional>
#include <string>

namespace titan {

class TeamInvitationDataEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id0_.encode(s); // == writeLong
        s.writeString(name_.has_value() ? &name_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        id0_ = LogicLong::decode(s);
        name_ = s.readString();
    }

    LogicLong id0_; // +0 (writeLong)
    std::optional<std::string> name_; // +8
};

} // namespace titan
