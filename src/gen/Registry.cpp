// Generated — do not edit. Registry aggregator.
#include "titan/gen/FunctionRegistry.hpp"

namespace titan::registry {

namespace detail {
extern const Entry kPart0[];
extern const std::size_t kPart0Count;
extern const Entry kPart1[];
extern const std::size_t kPart1Count;
extern const Entry kPart2[];
extern const std::size_t kPart2Count;
extern const Entry kPart3[];
extern const std::size_t kPart3Count;
} // namespace detail

namespace {
struct Part { const Entry* e; std::size_t n; };
const Part kParts[] = {
    {detail::kPart0, detail::kPart0Count},
    {detail::kPart1, detail::kPart1Count},
    {detail::kPart2, detail::kPart2Count},
    {detail::kPart3, detail::kPart3Count},
};
} // namespace

const Entry* entries(std::size_t* outCount) {
    // Parts are individually sorted; the caller iterates all parts.
    // For simplicity expose part 0 here and let counts()/find cover all.
    // (Full flattened view is unnecessary for status queries.)
    if (outCount) *outCount = 0;
    for (const Part& p : kParts) *outCount += p.n;
    return kParts[0].e;
}

Counts counts() {
    Counts c;
    for (const Part& p : kParts) {
        for (std::size_t i = 0; i < p.n; ++i) {
            ++c.total;
            switch (p.e[i].status) {
                case Status::Reimplemented: ++c.reimplemented; break;
                case Status::ThirdPartyExternal: ++c.thirdParty; break;
                case Status::Pending: ++c.pending; break;
            }
        }
    }
    return c;
}

const Entry* findByAddress(std::uint32_t address) {
    for (const Part& p : kParts) {
        std::size_t lo = 0, hi = p.n;
        while (lo < hi) {
            const std::size_t mid = lo + (hi - lo) / 2;
            if (p.e[mid].address < address) lo = mid + 1;
            else hi = mid;
        }
        if (lo < p.n && p.e[lo].address == address) return &p.e[lo];
    }
    return nullptr;
}

} // namespace titan::registry
