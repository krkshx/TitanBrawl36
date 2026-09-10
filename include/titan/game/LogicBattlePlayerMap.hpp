#pragma once

// LogicBattlePlayerMap::encode @0x972ee4 (const).
// Wire: encodeLogicLong @+0 (vint pair), string (nullable @+8), vint +16,
//   dataref (nullable @+24), LogicCompressedString @+32 (required),
//   encodeLogicLong @+40, string (nullable @+48), vint +56,
//   nullable LogicLong[] @+64 (null -> vint -1, else count + pairs).
// Decode mirrors the same order (stream ctor used by TeamEntry::decode).

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicCompressedString.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicBattlePlayerMap : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        encodeLogicLong(s, id0_);
        s.writeString(name8_.has_value() ? &name8_.value() : nullptr);
        s.writeVInt(v16_);
        DataReference::encodeNullable(s, ref24_);
        if (!compressed_) throw pending_reverse("LogicBattlePlayerMap needs LogicCompressedString");
        compressed_->encode(s);
        encodeLogicLong(s, id40_);
        s.writeString(name48_.has_value() ? &name48_.value() : nullptr);
        s.writeVInt(v56_);
        if (!longs_) {
            s.writeVInt(-1);
        } else {
            s.writeVInt(static_cast<i32>(longs_->size()));
            for (const auto& id : *longs_) encodeLogicLong(s, id);
        }
    }
    void decode(ByteStream& s) override {
        id0_.high = s.readVInt();
        id0_.low = s.readVInt();
        name8_ = s.readString();
        v16_ = s.readVInt();
        ref24_ = DataReference::decodeNullable(s);
        compressed_ = std::make_unique<LogicCompressedString>();
        compressed_->decode(s);
        id40_.high = s.readVInt();
        id40_.low = s.readVInt();
        name48_ = s.readString();
        v56_ = s.readVInt();
        const i32 n = s.readVInt();
        if (n < 0) {
            longs_.reset();
        } else {
            longs_.emplace();
            for (i32 i = 0; i < n; ++i) {
                LogicLong id;
                id.high = s.readVInt();
                id.low = s.readVInt();
                longs_->push_back(id);
            }
        }
    }

    LogicLong id0_; // +0 (vint pair)
    std::optional<std::string> name8_; // +8
    i32 v16_ = 0;
    std::optional<DataReference> ref24_; // +24
    std::unique_ptr<LogicCompressedString> compressed_; // +32 (required)
    LogicLong id40_; // +40 (vint pair)
    std::optional<std::string> name48_; // +48
    i32 v56_ = 0;
    std::optional<std::vector<LogicLong>> longs_; // +64 (null -> -1)
};

} // namespace titan
