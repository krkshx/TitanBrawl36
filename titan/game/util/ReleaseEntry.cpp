#pragma once

// ReleaseEntry::encode @0x3ef33c.
// Wire: dataref (nullable LogicData* @+0), int @+8, int @+12.
// Decode is the stream ctor used by LogicConfData::decode: same reads
// in the same order (mirrored below).

#include "titan/core/DataReference.cpp"
#include "titan/messages/Nested.cpp"

#include <optional>

namespace titan {

class ReleaseEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        DataReference::encodeNullable(s, ref_);
        s.writeInt(v8_);
        s.writeInt(v12_);
    }
    void decode(ByteStream& s) override {
        ref_ = DataReference::decodeNullable(s);
        v8_ = s.readInt();
        v12_ = s.readInt();
    }

    std::optional<DataReference> ref_;
    i32 v8_ = 0;
    i32 v12_ = 0;
};

} // namespace titan
