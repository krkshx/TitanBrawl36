#pragma once

// AllianceWarNode::encode @0x5f7f9c (const).
// Wire: vint +0/+4/+8/+12, dataref (nullable @+16), vint +24/+28/+32,
//   vint count + int[] (@+40, count +52).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>
#include <vector>

namespace titan {

class AllianceWarNode : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v4_);
        s.writeVInt(v8_);
        s.writeVInt(v12_);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
        s.writeVInt(v32_);
        s.writeVInt(static_cast<i32>(ids_.size()));
        for (const i32 v : ids_) s.writeVInt(v);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v4_ = s.readVInt();
        v8_ = s.readVInt();
        v12_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
        v24_ = s.readVInt();
        v28_ = s.readVInt();
        v32_ = s.readVInt();
        const i32 n = s.readVInt();
        ids_.clear();
        for (i32 i = 0; i < n; ++i) ids_.push_back(s.readVInt());
    }

    i32 v0_ = 0, v4_ = 0, v8_ = 0, v12_ = 0;
    std::optional<DataReference> ref_; // +16
    i32 v24_ = 0, v28_ = 0, v32_ = 0;
    std::vector<i32> ids_; // +40 (+52)
};

} // namespace titan
