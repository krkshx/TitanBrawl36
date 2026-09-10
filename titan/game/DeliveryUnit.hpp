#pragma once

// DeliveryUnit::encode @0x4481e8 (const).
// Wire: vint @+8, vint count + GatchaDrop[] (LogicArrayList @+16, count +28).
// Decode mirrors the same order (stream ctor used by callers).

#include "titan/game/GatchaDrop.hpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <vector>

namespace titan {

class DeliveryUnit : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeVInt(v8_);
        s.writeVInt(static_cast<i32>(drops_.size()));
        for (const auto& d : drops_) d->encode(s);
    }
    void decode(ByteStream& s) override {
        v8_ = s.readVInt();
        const i32 n = s.readVInt();
        drops_.clear();
        for (i32 i = 0; i < n; ++i) {
            auto d = std::make_unique<GatchaDrop>();
            d->decode(s);
            drops_.push_back(std::move(d));
        }
    }

    i32 v8_ = 0;
    std::vector<std::unique_ptr<GatchaDrop>> drops_; // +16 (+28)
};

} // namespace titan
