#pragma once

// LogicBitList — fixed-size bit set backed by an int array.
// Layout: +0 intCount, +8 words*, +16 cached bits-set count.
//   C2(int bits) @0x7c7788: intCount = (bits + 31) / 32, zeroed.
//   encode @0x9740d8 (const): intCount × writeInt, NO count prefix.
//   decode @0x5c241c: intCount must be preset (callers construct C2(size)
//     first, e.g. BrawlPassSeasonData's stream ctor @0x2474f0 always uses
//     C2(128)); reads that many ints, then refreshes the cached popcount
//     via LogicMath::getBitsInInteger (plain popcount here).

#include "titan/messages/Nested.cpp"

#include <cstdint>
#include <vector>

namespace titan {

class LogicBitList : public NestedEntry {
public:
    explicit LogicBitList(i32 bits = 0) { resizeBits(bits); }

    void encode(ByteStream& s) const override {
        for (const i32 w : words_) s.writeInt(w);
    }
    void decode(ByteStream& s) override {
        for (i32& w : words_) w = s.readInt();
        cachedBits_ = 0;
        for (const i32 w : words_) cachedBits_ += popcount(w);
    }

    void resizeBits(i32 bits) {
        const i32 n = (bits + 31) / 32;
        words_.assign(static_cast<std::size_t>(n), 0);
        cachedBits_ = 0;
    }
    [[nodiscard]] i32 intCount() const { return static_cast<i32>(words_.size()); }
    [[nodiscard]] i32 cachedBits() const { return cachedBits_; }
    std::vector<i32>& words() { return words_; }

private:
    static int popcount(i32 w) {
        std::uint32_t x = static_cast<std::uint32_t>(w);
        int c = 0;
        while (x != 0) {
            x &= x - 1;
            ++c;
        }
        return c;
    }

    std::vector<i32> words_;
    i32 cachedBits_ = 0;
};

} // namespace titan
