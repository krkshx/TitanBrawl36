#pragma once

// LogicConfData::encode @0x653b94, decode @0x6f6508.
// Order below follows the binary exactly (counts + arrays):
//   vint +0,
//   vint count + EventSlot[] (@+8, count +20),
//   vint count + EventData[] (@+24, count +36),
//   vint count + EventData[] (@+40, count +52),
//   vint count + int[] (@+56, count +68),
//   vint count + int[] (@+72, count +84),
//   vint count + int[] (@+88, count +100),
//   bool +104,
//   vint count + ReleaseEntry[] (@+112, count +124),
//   vint count + IntValueEntry[] (@+128, count +140),
//   vint count + TimedIntValueEntry[] (@+144, count +156),
//   vint count + CustomEvent[] (@+160, count +172).

#include "titan/game/CustomEvent.hpp"
#include "titan/game/EventData.hpp"
#include "titan/game/EventSlot.hpp"
#include "titan/game/IntValueEntry.hpp"
#include "titan/game/ReleaseEntry.hpp"
#include "titan/game/TimedIntValueEntry.hpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <vector>

namespace titan {

class LogicConfData : public NestedEntry {
public:
    void encode(ByteStream& s) const override;
    void decode(ByteStream& s) override;

    // getLevelUpCost @0x708b44: ints17_[level-1] when level <= size, else 0.
    [[nodiscard]] i32 getLevelUpCost(int level) const {
        if (level < 1 || level > static_cast<int>(ints17_.size())) return 0;
        return ints17_[static_cast<std::size_t>(level - 1)];
    }
    // getIntValueEntry @0x4698d8: first intValues_ entry with a_ == id.
    // getIntValue @0x51a1f0: entry value or getDefaultIntValue(id).
    // getIntValue(id, fallback) @0x26187c: entry value or caller default.
    // getDefaultIntValue @0x999c84: hardcoded table below.
    [[nodiscard]] const IntValueEntry* getIntValueEntry(i32 id) const;
    [[nodiscard]] i32 getIntValue(i32 id) const;
    [[nodiscard]] i32 getIntValue(i32 id, i32 fallback) const;
    [[nodiscard]] static i32 getDefaultIntValue(i32 id);

    i32 v0_ = 0; // +0
    std::vector<std::unique_ptr<EventSlot>> slots_;          // +8 (+20)
    std::vector<std::unique_ptr<EventData>> events1_;        // +24 (+36)
    std::vector<std::unique_ptr<EventData>> events2_;        // +40 (+52)
    std::vector<i32> ints17_;                                // +56 (+68)
    std::vector<i32> ints21_;                                // +72 (+84)
    std::vector<i32> ints25_;                                // +88 (+100)
    bool b104_ = false;                                      // +104
    std::vector<std::unique_ptr<ReleaseEntry>> releases_;    // +112 (+124)
    std::vector<std::unique_ptr<IntValueEntry>> intValues_;  // +128 (+140)
    std::vector<std::unique_ptr<TimedIntValueEntry>> timed_; // +144 (+156)
    std::vector<std::unique_ptr<CustomEvent>> customs_;      // +160 (+172)
};

} // namespace titan
