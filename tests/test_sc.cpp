// Self-check for the sc engine port (starts with Debugger).

#include "titan/sc/Debugger.hpp"
#include "titan/sc/DisplayObject.hpp"
#include "titan/sc/Sprite.hpp"

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
    // Sprite children (@0x2b0b90/@0x687b14/@0x27bcb4/@0x2bddd8/@0x35ae34).
    {
        titan::sc::Sprite root;
        titan::sc::DisplayObject a, b, c;
        CHECK(root.childCount() == 0 && a.parent_ == nullptr && a.index_ == -1);
        root.addChild(&a);
        root.addChild(&b);
        CHECK(root.childCount() == 2 && a.index_ == 0 && b.index_ == 1);
        CHECK(a.parent_ == &root && root.children()[1] == &b);
        root.addChildAt(&c, 1); // insert middle, followers shift
        CHECK(root.childCount() == 3 && c.index_ == 1 && b.index_ == 2);
        CHECK(root.children()[0] == &a && root.children()[2] == &b);
        root.removeChildAt(0); // detach head
        CHECK(a.parent_ == nullptr && a.index_ == -1);
        CHECK(root.childCount() == 2 && c.index_ == 0 && b.index_ == 1);
        root.removeChild(&c);
        CHECK(root.childCount() == 1 && c.parent_ == nullptr);
        root.removeAllChildren();
        CHECK(root.childCount() == 0 && b.parent_ == nullptr && b.index_ == -1);
    }
    {
        // Reparenting detaches from the old parent first.
        titan::sc::Sprite r1, r2;
        titan::sc::DisplayObject a;
        r1.addChild(&a);
        r2.addChild(&a);
        CHECK(r1.childCount() == 0 && r2.childCount() == 1 && a.parent_ == &r2);
    }

    if (failures == 0) std::puts("sc: all ok");
    return failures == 0 ? 0 : 1;
}
