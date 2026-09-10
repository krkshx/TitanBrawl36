#pragma once

// CustomEvent::encode @0x911588.
// Wire: vint @+0, vint @+4, ChronosTextEntry @+8/@+16/@+24 (all required;
// the binary calls encode directly with no null check).
// Decode is the stream ctor used by LogicConfData::decode: same reads
// in the same order (mirrored below).

#include "titan/game/ChronosTextEntry.hpp"
#include "titan/messages/Nested.cpp"

#include <memory>

namespace titan {

class CustomEvent : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v0_);
        s.writeVInt(v1_);
        if (!t0_ || !t1_ || !t2_) throw pending_reverse("CustomEvent needs all texts");
        t0_->encode(s);
        t1_->encode(s);
        t2_->encode(s);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readVInt();
        v1_ = s.readVInt();
        t0_ = std::make_unique<ChronosTextEntry>();
        t0_->decode(s);
        t1_ = std::make_unique<ChronosTextEntry>();
        t1_->decode(s);
        t2_ = std::make_unique<ChronosTextEntry>();
        t2_->decode(s);
    }

    i32 v0_ = 0;
    i32 v1_ = 0;
    std::unique_ptr<ChronosTextEntry> t0_, t1_, t2_;
};

} // namespace titan
