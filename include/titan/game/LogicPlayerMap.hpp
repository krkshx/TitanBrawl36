#pragma once

// LogicPlayerMap::encode @0x54ffb4, decode @0x6e4c14.
// Wire: logiclong, string, vint, dataref, LogicCompressedString,
// logiclong, string, vint, int64, vint x5.
// Split out of the entry wave; wire format unchanged.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicCompressedString.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <string>

namespace titan {

class LogicPlayerMap : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        s.writeString(name_ ? &*name_ : nullptr);
        s.writeVInt(v16_);
        DataReference::encodeNullable(s, ref_);
        if (!data_) throw pending_reverse("LogicPlayerMap needs LogicCompressedString");
        data_->encode(s);
        id2_.encode(s);
        s.writeString(s48_ ? &*s48_ : nullptr);
        s.writeVInt(v56_);
        s.writeLongLong(q64_);
        s.writeVInt(v72_);
        s.writeVInt(v76_);
        s.writeVInt(v80_);
        s.writeVInt(v84_);
        s.writeVInt(v88_);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        name_ = s.readString();
        v16_ = s.readVInt();
        ref_ = DataReference::decodeNullable(s);
        data_ = std::make_unique<LogicCompressedString>();
        data_->decode(s);
        id2_ = LogicLong::decode(s);
        s48_ = s.readString();
        v56_ = s.readVInt();
        q64_ = s.readLongLong();
        v72_ = s.readVInt();
        v76_ = s.readVInt();
        v80_ = s.readVInt();
        v84_ = s.readVInt();
        v88_ = s.readVInt();
    }
    LogicLong id_, id2_;
    std::optional<std::string> name_, s48_;
    i32 v16_ = 0, v56_ = 0, v72_ = 0, v76_ = 0, v80_ = 0, v84_ = 0, v88_ = 0;
    std::optional<DataReference> ref_;
    std::unique_ptr<LogicCompressedString> data_;
    i64 q64_ = 0;
};

} // namespace titan
