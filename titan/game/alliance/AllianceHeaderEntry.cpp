#pragma once

// AllianceHeaderEntry::encode @0x88a094, decode @0x8881cc.
// Wire: logiclong, string, dataref, vint x4, dataref, string, vint, bool
// (badge dataref + name string are out-of-order vs layout).
// Split out of AllianceEntries.hpp; wire format unchanged.

#include "titan/core/DataReference.cpp"
#include "titan/core/LogicLong.cpp"
#include "titan/messages/Nested.cpp"

#include <optional>
#include <string>

namespace titan {

class AllianceHeaderEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        s.writeString(name_ ? &*name_ : nullptr);
        DataReference::encodeNullable(s, dataRef1_);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        DataReference::encodeNullable(s, badgeRef_);
        s.writeString(description_ ? &*description_ : nullptr);
        s.writeVInt(v40_);
        s.writeBoolean(flag_);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        name_ = s.readString();
        dataRef1_ = DataReference::decodeNullable(s);
        v24_ = s.readVInt();
        v28_ = s.readVInt();
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        badgeRef_ = DataReference::decodeNullable(s);
        description_ = s.readString();
        v40_ = s.readVInt();
        flag_ = s.readBoolean();
    }
    LogicLong id_;
    std::optional<std::string> name_, description_;
    std::optional<DataReference> dataRef1_, badgeRef_;
    i32 v24_ = 0, v28_ = 0, v32_ = 0, v36_ = 0, v40_ = 0;
    bool flag_ = false;
};

} // namespace titan
