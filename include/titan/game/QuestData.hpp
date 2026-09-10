#pragma once

// QuestData::encode @0x467740.
// Wire: 10 × vint @+0..+36, bool @+40, bool @+41,
//   dataref (nullable LogicData* @+48), vint @+56/+60/+64, vint @+44 last.
// Decode mirrors the same order (stream ctor used by LogicQuests::decode).

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>

namespace titan {

class QuestData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        for (const i32 v : v_) s.writeVInt(v);
        s.writeBoolean(b40_);
        s.writeBoolean(b41_);
        DataReference::encodeNullable(s, ref_);
        s.writeVInt(v56_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
        s.writeVInt(v44_);
    }
    void decode(ByteStream& s) override {
        for (i32& v : v_) v = s.readVInt();
        b40_ = s.readBoolean();
        b41_ = s.readBoolean();
        ref_ = DataReference::decodeNullable(s);
        v56_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
        v44_ = s.readVInt();
    }

    i32 v_[10] = {}; // +0..+36
    bool b40_ = false, b41_ = false;
    std::optional<DataReference> ref_; // +48
    i32 v56_ = 0, v60_ = 0, v64_ = 0;
    i32 v44_ = 0; // wired last
};

} // namespace titan
