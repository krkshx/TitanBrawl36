#pragma once

// LogicDataSlot::encode @0x7646e4, decode @0x2d49cc.
// Wire: dataref + vint count.
// Split out of LogicClientAvatar.hpp; wire format unchanged.

#include "titan/core/DataReference.hpp"
#include "titan/messages/Nested.cpp"

#include <optional>

namespace titan {

class LogicDataSlot : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        DataReference::encodeNullable(s, data_);
        s.writeVInt(count_);
    }
    void decode(ByteStream& s) override {
        data_ = DataReference::decodeNullable(s);
        count_ = s.readVInt();
    }
    std::optional<DataReference> data_;
    i32 count_ = 0;
};

} // namespace titan
