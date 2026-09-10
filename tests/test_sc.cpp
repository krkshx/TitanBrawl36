// Self-check for the sc engine port (starts with Debugger).

#include "titan/sc/Debugger.hpp"
#include "titan/sc/DisplayObject.hpp"

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

    // DisplayObject transform core (offsets @0x529c3c family).
    {
        titan::sc::DisplayObject o;
        CHECK(o.getX() == 0.0f && o.getY() == 0.0f);
        CHECK(o.getScaleX() == 1.0f && o.getScaleY() == 1.0f);
        o.setX(10.5f);
        CHECK(o.getX() == 10.5f);
        o.setXY(-3.0f, 4.0f);
        CHECK(o.getX() == -3.0f && o.getY() == 4.0f);
        o.setScaleX(2.0f);
        CHECK(o.getScaleX() == 2.0f && o.getScaleY() == 1.0f);
        o.setScaleY(3.0f);
        CHECK(o.getScaleY() == 3.0f);
        o.setScale(0.5f);
        CHECK(o.getScaleX() == 0.5f && o.getScaleY() == 0.5f);
        CHECK(o.isVisible());
        o.setVisible(false);
        CHECK(!o.isVisible());
        o.setVisibleRecursive(true);
        CHECK(o.isVisible());
    }

    if (failures == 0) std::puts("sc: all ok");
    return failures == 0 ? 0 : 1;
}
