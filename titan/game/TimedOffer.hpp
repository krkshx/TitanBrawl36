#pragma once

// TimedOffer::encode @0x354c54.
// Wire: dataref (nullable LogicData* @+0), int @+8, int @+12 (size 0x10).
// Decode is the stream ctor called from LogicDailyData::decode @0x4360d8 /
// @0x436104: same reads in the same order (mirrored below).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.cpp"

#include <optional>

namespace titan {

class TimedOffer : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        DataReference::encodeNullable(s, offer_);
        s.writeInt(v8_);
        s.writeInt(v12_);
    }
    void decode(ByteStream& s) override {
        offer_ = DataReference::decodeNullable(s);
        v8_ = s.readInt();
        v12_ = s.readInt();
    }

    std::optional<DataReference> offer_;
    i32 v8_ = 0;
    i32 v12_ = 0;
};

} // namespace titan
