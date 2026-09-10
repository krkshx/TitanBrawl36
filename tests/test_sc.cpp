// Self-check for the sc engine port (starts with Debugger).

#include "titan/sc/Debugger.hpp"

#include <cstdio>

using namespace titan::sc;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    // error @0x569b9c / warning @0x254e64: count, never throw.
    Debugger::resetCounters();
    CHECK(Debugger::errorCount() == 0 && Debugger::warningCount() == 0);
    Debugger::error("boom");
    Debugger::error("boom2");
    Debugger::warning("careful");
    CHECK(Debugger::errorCount() == 2);
    CHECK(Debugger::warningCount() == 1);
    Debugger::resetCounters();
    CHECK(Debugger::errorCount() == 0 && Debugger::warningCount() == 0);

    if (failures == 0) std::puts("sc: all ok");
    return failures == 0 ? 0 : 1;
}
