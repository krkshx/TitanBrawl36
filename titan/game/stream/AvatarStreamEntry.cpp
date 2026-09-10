#pragma once

// AvatarStreamEntry base — reversed from libg_decrypted.so (ARM64).
// encode @0x3fe820, decode @0x3f8140.
// Wire: logiclong +8, logiclong +16, string +24, vint +32/+36,
// bool +40/+41 (one more bool than StreamEntry).

#include "titan/core/LogicLong.cpp"
#include "titan/messages/Nested.cpp"

#include <optional>
#include <string>

namespace titan {

class AvatarStreamEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id1_.encode(s);
        id2_.encode(s);
        s.writeString(name_ ? &*name_ : nullptr);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        s.writeBoolean(b40_);
        s.writeBoolean(b41_);
    }
    void decode(ByteStream& s) override {
        id1_ = LogicLong::decode(s);
        id2_ = LogicLong::decode(s);
        name_ = s.readString();
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        b40_ = s.readBoolean();
        b41_ = s.readBoolean();
    }
    LogicLong id1_, id2_;
    std::optional<std::string> name_;
    i32 v32_ = 0, v36_ = 0;
    bool b40_ = false, b41_ = false;
};

} // namespace titan
