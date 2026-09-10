#pragma once

// CooldownEntry::encode @0x5402a0.
// Wire: vint @+0, dataref (nullable LogicData* @+8), vint @+16.
// (No standalone decode symbol; decode is the stream ctor called from
// LogicDailyData::decode: same reads in the same order, mirrored below.)

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.cpp"

#include <optional>

namespace titan {

class CooldownEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(id_);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(value_);
    }
    void decode(ByteStream& s) override {
        id_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
        value_ = s.readVInt();
    }

    i32 id_ = 0;
    std::optional<DataReference> ref_;
    i32 value_ = 0;
};

} // namespace titan
