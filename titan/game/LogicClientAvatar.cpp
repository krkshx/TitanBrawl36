// LogicClientAvatar commodity bodies — getCommodityCount @0x32922c,
// setCommodityCount @0x7c7bc8, commodityCountChangeHelper @0x5c3590.

#include "titan/game/LogicClientAvatar.hpp"

#include "titan/game/LogicResourceData.hpp"
#include "titan/sc/Debugger.hpp"

#include <limits>

namespace titan {

namespace {

bool refEquals(const std::optional<DataReference>& ref, const LogicData& data) {
    return ref.has_value() && ref->classId == data.classId()
           && ref->instanceId == data.row();
}

} // namespace

i32 LogicClientAvatar::getCommodityCount(int slot, const LogicData& data) const {
    if (slot < 0 || slot >= 8) {
        sc::Debugger::error("LogicClientAvatar::getCommodityCount: bad slot");
        return 0;
    }
    for (const auto& s : slots_[slot]) {
        if (refEquals(s.data_, data)) return s.count_;
    }
    return 0;
}

void LogicClientAvatar::setCommodityCount(int slot, const LogicData& data, i32 count) {
    if (slot < 0 || slot >= 8) {
        // Binary errors twice here (and a third time below when found).
        sc::Debugger::error("LogicClientAvatar::setCommodityCount: bad slot");
        sc::Debugger::error("LogicClientAvatar::setCommodityCount: bad slot");
    }
    if (slot < 0 || slot >= 8) return;
    for (auto& s : slots_[slot]) {
        if (refEquals(s.data_, data)) {
            s.count_ = count;
            return;
        }
    }
    LogicDataSlot fresh;
    fresh.data_ = DataReference{data.classId(), data.row()};
    fresh.count_ = count;
    slots_[slot].push_back(fresh);
}

int LogicClientAvatar::commodityCountChangeHelper(int slot, const LogicData& data,
                                                 i32 delta, int a5, bool noCap,
                                                 int a7) {
    (void)a5;
    (void)a7; // listener-only args; notifications skipped (platform)
    if (slot < 0 || slot > 7) {
        sc::Debugger::error("LogicClientAvatar::commodityCountChangeHelper: bad slot");
        return 0;
    }
    if (delta == 0) return 0;
    LogicDataSlot* found = nullptr;
    for (auto& s : slots_[slot]) {
        if (refEquals(s.data_, data)) {
            found = &s;
            break;
        }
    }
    if (!found) {
        LogicDataSlot fresh;
        fresh.data_ = DataReference{data.classId(), data.row()};
        fresh.count_ = 0;
        slots_[slot].push_back(fresh);
        found = &slots_[slot].back();
    }
    int applied = delta;
    int updated = found->count_ + delta;
    if (delta >= 1 && data.getDataType() == 5 && !noCap
        && data.dataTables() != nullptr) {
        const LogicResourceData res(data.dataTables(), data.row());
        const int cap = res.getCap();
        if (cap >= 1) {
            const int capped = updated < cap ? updated : cap;
            applied = capped - found->count_;
            updated = capped;
        }
    }
    if (found->count_ >= 1 && delta >= 1 && updated < 0) {
        updated = std::numeric_limits<i32>::max();
    }
    found->count_ = updated;
    return applied;
}

} // namespace titan
