#pragma once

// ChronosFileEntry::encode @0x639e40.
// Wire: 2 × writeStringReference of the EMBEDDED String objects @+0/@+16
// (vtable slot +0x20, cf. ChronosTextEntry @0x7b6d10). Never null.
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/messages/Nested.cpp"

#include <string>

namespace titan {

class ChronosFileEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeStringReference(s0_);
        s.writeStringReference(s16_);
    }
    void decode(ByteStream& s) override {
        s0_ = s.readStringReference();
        s16_ = s.readStringReference();
    }

    std::string s0_, s16_;
};

} // namespace titan
