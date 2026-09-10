#pragma once

// AddableFriendEntry::encode @0x419d80, decode @0x58f924.
// Wire order: int +0, int +4, string +24, int +32, string +8, string +16.
// Split out of the entry wave; wire format unchanged.

#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

class AddableFriendEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeInt(v0_);
        s.writeInt(v4_);
        s.writeString(s24_ ? &*s24_ : nullptr);
        s.writeInt(v32_);
        s.writeString(s8_ ? &*s8_ : nullptr);
        s.writeString(s16_ ? &*s16_ : nullptr);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readInt();
        v4_ = s.readInt();
        s24_ = s.readString();
        v32_ = s.readInt();
        s8_ = s.readString();
        s16_ = s.readString();
    }
    i32 v0_ = 0, v4_ = 0, v32_ = 0;
    std::optional<std::string> s24_, s8_, s16_;
};

} // namespace titan
