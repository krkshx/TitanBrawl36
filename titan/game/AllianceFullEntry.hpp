#pragma once

// AllianceFullEntry::encode @0x49a4bc, decode @0x253a58.
// Wire: header, string, vint count (-1=null) + member loop.
// Split out of AllianceEntries.hpp; wire format unchanged.

#include "titan/game/AllianceHeaderEntry.hpp"
#include "titan/game/AllianceMemberEntry.hpp"
#include "titan/messages/Nested.cpp"

#include <optional>
#include <string>
#include <vector>

namespace titan {

class AllianceFullEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        header_.encode(s);
        s.writeString(description_ ? &*description_ : nullptr);
        if (isNull_) {
            s.writeVInt(-1);
            return;
        }
        s.writeVInt(static_cast<i32>(members_.size()));
        for (const auto& m : members_) m.encode(s);
    }
    void decode(ByteStream& s) override {
        header_.decode(s);
        description_ = s.readString();
        const i32 n = s.readVInt();
        members_.clear();
        isNull_ = (n < 0);
        for (i32 i = 0; i < n; ++i) {
            AllianceMemberEntry m;
            m.decode(s);
            members_.push_back(m);
        }
    }
    AllianceHeaderEntry header_;
    std::optional<std::string> description_;
    std::vector<AllianceMemberEntry> members_;
    bool isNull_ = false;
};

} // namespace titan
