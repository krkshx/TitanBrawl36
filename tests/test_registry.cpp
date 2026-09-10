// Registry self-check: every IDA function listed exactly once,
// known splits add up, and spot-checks resolve by address.

#include "titan/gen/FunctionRegistry.cpp"

#include <cstdio>

using namespace titan::registry;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    const Counts c = counts();
    std::printf("registry: total=%zu reimplemented=%zu thirdparty=%zu pending=%zu\n",
                c.total, c.reimplemented, c.thirdParty, c.pending);
    CHECK(c.total == 37064);
    CHECK(c.total == c.reimplemented + c.thirdParty + c.pending);
    CHECK(c.reimplemented >= 22);

    // Spot checks by IDA address.
    const Entry* e = findByAddress(0x298F64); // ByteStream::writeVInt
    CHECK(e != nullptr && e->status == Status::Reimplemented);
    e = findByAddress(0x8F2E70); // PiranhaMessage::encode
    CHECK(e != nullptr && e->status == Status::Reimplemented);
    e = findByAddress(0x4465CC); // ResetAccountMessage::encode
    CHECK(e != nullptr && e->status == Status::Reimplemented);
    e = findByAddress(0xDEADBEEF); // not a function
    CHECK(e == nullptr);

    if (failures == 0) std::puts("registry: all ok");
    return failures == 0 ? 0 : 1;
}
