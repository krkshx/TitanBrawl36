#pragma once

// LogicClientAvatar::encode/decode (field order verified against binary).
// Wire: 3x logiclong, stringref name, bool, int, vint(8, version-checked),
// 8x (vint count + LogicDataSlot array), 9x vint.
// Split out of the old LogicClientAvatar.hpp; wire format unchanged.

#include "titan/core/LogicLong.hpp"
#include "titan/game/LogicDataSlot.hpp"
#include "titan/messages/Nested.hpp"

#include <string>
#include <vector>

namespace titan {

class LogicClientAvatar : public NestedEntry {
public:
    static constexpr i32 kVersion = 8;

    void encode(ByteStream& s) const override {
        id1_.encode(s);
        id2_.encode(s);
        id3_.encode(s);
        s.writeStringReference(name_);
        s.writeBoolean(flag_);
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
    }
    void decode(ByteStream& s) override {
        id1_ = LogicLong::decode(s);
        id2_ = LogicLong::decode(s);
        id3_ = LogicLong::decode(s);
        name_ = s.readStringReference();
        flag_ = s.readBoolean();
        v196_ = s.readInt();
        const i32 ver = s.readVInt(); // binary checks == 8
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
    }
    LogicLong id1_, id2_, id3_;
    std::string name_;
    bool flag_ = false;
    i32 v196_ = 0;
    std::vector<LogicDataSlot> slots_[8];
    i32 v144_ = 0, v148_ = 0, v136_ = 0, v140_ = 0, v152_ = 0;
    i32 v168_ = 0, v172_ = 0, v176_ = 0, v180_ = 0;

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
};

} // namespace titan
