#pragma once

// PlayerDisplayData::encode @0x934728, decode @0x56a900 (stream ctor).
// Wire: string name, vint x4 (unk, thumbnailId, nameColorId, unk=-1).
// Split out of AllianceEntries.hpp; wire format unchanged.

#include "titan/messages/Nested.cpp"

#include <optional>
#include <string>

namespace titan {

class PlayerDisplayData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeString(name_ ? &*name_ : nullptr);
        s.writeVInt(v16_);
        s.writeVInt(thumbnailId_);
        s.writeVInt(nameColorId_);
        s.writeVInt(v28_);
    }
    void decode(ByteStream& s) override {
        name_ = s.readString();
        v16_ = s.readVInt();
        thumbnailId_ = s.readVInt();
        nameColorId_ = s.readVInt();
        v28_ = s.readVInt();
    }
    std::optional<std::string> name_;
    i32 v16_ = 0, thumbnailId_ = 0, nameColorId_ = 0, v28_ = -1;
};

} // namespace titan
