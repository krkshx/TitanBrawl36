#pragma once

// HeroDataEntry::encode @0x72d8ec (const).
// Wire: dataref (nullable @+0), dataref (nullable @+8), vint @+16,
//   bool @+20, string (nullable String* @+24).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/core/DataReference.cpp"
#include "titan/messages/Nested.cpp"

#include <optional>
#include <string>

namespace titan {

class HeroDataEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        DataReference::encodeNullable(s, ref0_);
        DataReference::encodeNullable(s, ref8_);
        s.writeVInt(v16_);
        s.writeBoolean(b20_);
        s.writeString(str_.has_value() ? &str_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        ref0_ = DataReference::decodeNullable(s);
        ref8_ = DataReference::decodeNullable(s);
        v16_ = s.readVInt();
        b20_ = s.readBoolean();
        str_ = s.readString();
    }

    std::optional<DataReference> ref0_, ref8_;
    i32 v16_ = 0;
    bool b20_ = false;
    std::optional<std::string> str_; // +24
};

} // namespace titan
