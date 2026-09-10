#pragma once

// VanityItems::encode @0x5980fc.
// Wire: vint count + VanityItemEntry[] (LogicArrayList @+0, count +12).
// Decode is the stream ctor used by LogicDailyData::decode: same reads
// in the same order (mirrored below).
//
// has @0x448ce8 / add @0x8a55b4 match entries by LogicData POINTER
// identity (tables own singleton rows); our port compares (classId, row),
// which is equivalent. add() creates the entry with a single (1,1) prop.

#include "titan/game/data/LogicData.cpp"
#include "titan/messages/Nested.cpp"
#include "titan/game/avatar/VanityItemEntry.cpp"
#include "titan/game/avatar/VanityItemProp.cpp"

#include <memory>
#include <vector>

namespace titan {

class VanityItems : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(static_cast<i32>(items_.size()));
        for (const auto& e : items_) e->encode(s);
    }
    void decode(ByteStream& s) override {
        const i32 n = s.readVInt();
        items_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto e = std::make_unique<VanityItemEntry>();
            e->decode(s);
            items_.push_back(std::move(e));
        }
    }

    std::vector<std::unique_ptr<VanityItemEntry>> items_;

    [[nodiscard]] bool has(const LogicData& data) const {
        for (const auto& e : items_) {
            if (e && e->ref_.has_value()
                && e->ref_->classId == data.classId()
                && e->ref_->instanceId == data.row()) {
                return true;
            }
        }
        return false;
    }
    void add(const LogicData& data) {
        if (has(data)) return;
        auto e = std::make_unique<VanityItemEntry>();
        e->ref_ = DataReference{data.classId(), data.row()};
        auto p = std::make_unique<VanityItemProp>();
        p->a_ = 1;
        p->b_ = 1;
        e->props_.push_back(std::move(p));
        items_.push_back(std::move(e));
    }
};

} // namespace titan
