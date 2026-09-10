#pragma once

// LogicClientAvatar + LogicDataSlot — reversed from libg_decrypted.so (ARM64).

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>
#include <vector>

namespace titan {

// LogicDataSlot::encode @0x7646e4, decode @0x2d49cc.
// Wire: dataref + vint count.
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

// LogicClientAvatar::encode/decode (field order verified against binary).
// Wire: 3x logiclong, stringref name, bool, int, vint(8, version-checked),
// 8x (vint count + LogicDataSlot array), 9x vint.
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
};

} // namespace titan
