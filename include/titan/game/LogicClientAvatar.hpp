#pragma once

// LogicClientAvatar::encode @0x900668, decode @0x6e6688.
// Wire: 3x encodeLogicLong (vint pairs, NOT LogicLong::encode ints),
// stringref name, bool nameSetByUser @+192, int @+196, vint(8)
// (decode errors unless == 8), 8x (vint count + LogicDataSlot array),
// 12x vint (+144/+148/+136/+140/+152/+168/+172/+176/+180/+184/+188/+200).
// NOTE: an earlier revision wrote ids as fixed ints and only 9 tail
// vints — corrected against both functions above.

#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicData.hpp"
#include "titan/game/LogicDataSlot.hpp"
#include "titan/messages/Nested.hpp"
#include "titan/sc/Debugger.hpp"

#include <string>
#include <vector>

namespace titan {

class LogicClientAvatar : public NestedEntry {
public:
    static constexpr i32 kVersion = 8;

    void encode(ByteStream& s) const override {
        encodeLogicLong(s, id1_);
        encodeLogicLong(s, id2_);
        encodeLogicLong(s, id3_);
        s.writeStringReference(name_);
        s.writeBoolean(nameSetByUser_);
        s.writeInt(v196_);
        s.writeVInt(kVersion);
        for (const auto& arr : slots_) {
            s.writeVInt(static_cast<i32>(arr.size()));
            for (const auto& slot : arr) slot.encode(s);
        }
        s.writeVInt(v144_);
        s.writeVInt(v148_);
        s.writeVInt(v136_);
        s.writeVInt(v140_);
        s.writeVInt(v152_);
        s.writeVInt(v168_);
        s.writeVInt(v172_);
        s.writeVInt(v176_);
        s.writeVInt(v180_);
        s.writeVInt(v184_);
        s.writeVInt(v188_);
        s.writeVInt(v200_);
    }
    void decode(ByteStream& s) override {
        id1_ = decodeLogicLong(s);
        id2_ = decodeLogicLong(s);
        id3_ = decodeLogicLong(s);
        name_ = s.readStringReference();
        nameSetByUser_ = s.readBoolean();
        v196_ = s.readInt();
        const i32 ver = s.readVInt(); // binary errors unless == 8
        (void)ver;
        for (auto& arr : slots_) {
            const i32 n = s.readVInt();
            arr.clear();
            for (i32 i = 0; i < n; ++i) {
                LogicDataSlot slot;
                slot.decode(s);
                arr.push_back(slot);
            }
        }
        v144_ = s.readVInt();
        v148_ = s.readVInt();
        v136_ = s.readVInt();
        v140_ = s.readVInt();
        v152_ = s.readVInt();
        v168_ = s.readVInt();
        v172_ = s.readVInt();
        v176_ = s.readVInt();
        v180_ = s.readVInt();
        v184_ = s.readVInt();
        v188_ = s.readVInt();
        v200_ = s.readVInt();
    }
    LogicLong id1_, id2_, id3_;
    std::string name_;
    bool nameSetByUser_ = false; // +192 (was misnamed flag_)
    i32 v196_ = 0;
    std::vector<LogicDataSlot> slots_[8];
    i32 v144_ = 0, v148_ = 0, v136_ = 0, v140_ = 0, v152_ = 0;
    i32 v168_ = 0, v172_ = 0, v176_ = 0, v180_ = 0, v184_ = 0, v188_ = 0;
    i32 v200_ = 0;

    // Named currency accessors (offsets verified via IDA):
    // diamonds +144 (get @0x6f4528, set @0x1bf950),
    // freeDiamonds +148 (get @0x771348, set @0x615f48),
    // cumulativePurchasedDiamonds +152 (add @0x908b84).
    [[nodiscard]] i32 getDiamonds() const { return v144_; }
    void setDiamonds(i32 v) { v144_ = v; }
    [[nodiscard]] i32 getFreeDiamonds() const { return v148_; }
    void setFreeDiamonds(i32 v) { v148_ = v; }
    void addCumulativePurchasedDiamonds(i32 v) { v152_ += v; }
    [[nodiscard]] i32 getCumulativePurchasedDiamonds() const { return v152_; }

    // useDiamonds @0x591f4c: diamonds -= n, free = max(0, free - n),
    // Debugger::error on negative input or negative result.
    // get/setNameSetByUser @0x613d30/@0x2326a8 (+192).
    void useDiamonds(i32 amount) {
        if (amount < 0) sc::Debugger::error("LogicClientAvatar::useDiamonds: negative");
        v144_ -= amount;
        const i32 free = v148_ - amount;
        v148_ = free & ~(free >> 31);
        if (v144_ < 0) sc::Debugger::error("LogicClientAvatar::useDiamonds: negative result");
    }
    [[nodiscard]] bool getNameSetByUser() const { return nameSetByUser_; }
    void setNameSetByUser(bool v) { nameSetByUser_ = v; }
    void setName(const std::string& name) { name_ = name; }

    // Commodity economy (slot arrays hold (dataref, count) pairs):
    // getCommodityCount @0x32922c: first matching slot count, else 0
    // (binary errors on slot >= 8 then reads OOB — ours errors + returns 0).
    // setCommodityCount @0x7c7bc8: set or append (binary errors twice on
    // slot >= 8 then writes OOB — ours errors twice and returns safely).
    // commodityCountChangeHelper @0x5c3590: add delta with resource-cap
    // (resources class 5, LogicResourceData::getCap) and INT_MAX overflow
    // guard; returns the actually applied delta; delta == 0 is a no-op.
    // Change-listener notifications skipped (platform).
    [[nodiscard]] i32 getCommodityCount(int slot, const LogicData& data) const;
    void setCommodityCount(int slot, const LogicData& data, i32 count);
    int commodityCountChangeHelper(int slot, const LogicData& data, i32 delta,
                                   int a5 = 0, bool noCap = false, int a7 = 0);
    // getHeroLevel @0x2b9b74: hero level = slot-5 count + 1 (1 if absent);
    // setHeroLevel @0x8a2ae0 = setCommodityCount(5, hero, level - 1).
    [[nodiscard]] i32 getHeroLevel(const LogicData& hero) const {
        for (const auto& s : slots_[5]) {
            if (s.data_.has_value() && s.data_->classId == hero.classId()
                && s.data_->instanceId == hero.row()) {
                return s.count_ + 1;
            }
        }
        return 1;
    }
    void setHeroLevel(const LogicData& hero, i32 level) {
        setCommodityCount(5, hero, level - 1);
    }
    // addFreeDiamonds @0x61f7c0: no-op on 0, error on negative, else
    // diamonds += n AND freeDiamonds += n (listener callback skipped).
    // addPaidDiamonds @0x73f334: no-op on 0, else diamonds += n AND
    // cumulativePurchasedDiamonds += n (NOT free).
    void addFreeDiamonds(i32 amount) {
        if (amount == 0) return;
        if (amount < 0) sc::Debugger::error("LogicClientAvatar::addFreeDiamonds: negative");
        v144_ += amount;
        v148_ += amount;
    }
    void addPaidDiamonds(i32 amount) {
        if (amount == 0) return;
        v144_ += amount;
        v152_ += amount;
    }
};

} // namespace titan
