#pragma once

// LatencyData::encode @0x726198.
// Wire: vint +0/+4/+8/+12, bool +16, writeLongLong @+24 (u64),
//   string (nullable String* @+32/+40/+48).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace titan {

class LatencyData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v4_);
        s.writeVInt(v8_);
        s.writeVInt(v12_);
        s.writeBoolean(b16_);
        s.writeLongLong(static_cast<i64>(ts_));
        s.writeString(s32_.has_value() ? &s32_.value() : nullptr);
        s.writeString(s40_.has_value() ? &s40_.value() : nullptr);
        s.writeString(s48_.has_value() ? &s48_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v4_ = s.readVInt();
        v8_ = s.readVInt();
        v12_ = s.readVInt();
        b16_ = s.readBoolean();
        ts_ = static_cast<u64>(s.readLongLong());
        s32_ = s.readString();
        s40_ = s.readString();
        s48_ = s.readString();
    }

    i32 v0_ = 0, v4_ = 0, v8_ = 0, v12_ = 0;
    bool b16_ = false;
    u64 ts_ = 0; // +24 (writeLongLong)
    std::optional<std::string> s32_, s40_, s48_;
};

} // namespace titan
