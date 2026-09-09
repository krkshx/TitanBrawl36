// Self-check for reimplemented messages (encode/decode symmetry).

#include "titan/messages/KeepAliveMessages.hpp"
#include "titan/messages/ResetAccountMessage.hpp"

#include <cassert>
#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    CHECK(KeepAliveMessage().getMessageType() == 10108);
    CHECK(KeepAliveServerMessage().getMessageType() == 20108);
    {
        KeepAliveMessage m;
        m.encode();
        CHECK(m.stream().size() == 0); // base encode writes nothing
    }
    CHECK(ResetAccountMessage().getMessageType() == 10116);
    {
        ResetAccountMessage out;
        out.setAccountPreset(7);
        out.encode();
        CHECK(out.stream().size() == 4); // single writeInt

        ResetAccountMessage in;
        in.stream().setBuffer(out.stream().data(), out.stream().size());
        in.decode();
        CHECK(in.accountPreset() == 7);
    }
    if (failures == 0) std::puts("messages: all ok");
    return failures == 0 ? 0 : 1;
}
