#pragma once

// StreamEntry base — reversed from libg_decrypted.so (ARM64).
// encode @0x550bec, decode @0x87b534.
// Wire: logiclong +8, logiclong +16, string +24, vint +32/+36, bool +40.
// Subclasses prepend this base, then add their fields; the entry type
// travels separately (per-entry vint in stream arrays).

#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

class StreamEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id1_.encode(s);
        id2_.encode(s);
        s.writeString(name_ ? &*name_ : nullptr);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        s.writeBoolean(b40_);
    }
    void decode(ByteStream& s) override {
        id1_ = LogicLong::decode(s);
        id2_ = LogicLong::decode(s);
        name_ = s.readString();
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        b40_ = s.readBoolean();
    }
    LogicLong id1_, id2_;
    std::optional<std::string> name_;
    i32 v32_ = 0, v36_ = 0;
    bool b40_ = false;
};

} // namespace titan
