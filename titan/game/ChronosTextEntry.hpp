#pragma once

// ChronosTextEntry::encode @0x7b6d10.
// Wire: int id @+0, stringref text @+8 (vtable slot +0x20, resolved via the
// ByteStream vtable @0xd5c468 to writeStringReference @0x608f14).
// Decode is the stream ctor used by LogicOfferBundle::decode: same reads
// in the same order (mirrored below).

#include "titan/messages/Nested.cpp"

#include <string>

namespace titan {

class ChronosTextEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeInt(id_);
        s.writeStringReference(text_);
    }
    void decode(ByteStream& s) override {
        id_ = s.readInt();
        text_ = s.readStringReference();
    }

    i32 id_ = 0;
    std::string text_;
};

} // namespace titan
