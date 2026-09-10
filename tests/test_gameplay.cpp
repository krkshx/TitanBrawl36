// Self-check for command execute() semantics (gameplay wave).

#include "titan/commands/LogicChangeAvatarNameCommand.hpp"
#include "titan/commands/LogicDiamondsAddedCommand.hpp"
#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicHomeMode.hpp"

#include <cstdio>
#include <memory>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

static std::unique_ptr<LogicHomeMode> makeHome(int diamonds, int free) {
    auto home = std::make_unique<LogicHomeMode>();
    auto avatar = std::make_unique<LogicClientAvatar>();
    avatar->setDiamonds(diamonds);
    avatar->setFreeDiamonds(free);
    home->setPlayerAvatar(std::move(avatar));
    return home;
}

int main() {
    // DiamondsAdded execute @0x44d194.
    {
        // No avatar -> -1.
        LogicDiamondsAddedCommand c;
        LogicHomeMode empty;
        CHECK(c.execute(&empty, 0, false) == -1);
        CHECK(c.execute(nullptr, 0, false) == -1);
    }
    {
        // Plain add (b_ false): diamonds grow, cumulative booked.
        auto home = makeHome(100, 10);
        LogicDiamondsAddedCommand c;
        c.b_ = false;
        c.i1_ = 50;
        CHECK(c.execute(home.get(), 0, false) == 0);
        const auto* av = home->getPlayerAvatar();
        CHECK(av->getDiamonds() == 150);
        CHECK(av->getFreeDiamonds() == 10);
        CHECK(av->getCumulativePurchasedDiamonds() == 50);
    }
    {
        // Free path (b_ true), positive: both grow, no purchase booking.
        auto home = makeHome(100, 10);
        LogicDiamondsAddedCommand c;
        c.b_ = true;
        c.i1_ = 50;
        CHECK(c.execute(home.get(), 0, false) == 0);
        const auto* av = home->getPlayerAvatar();
        CHECK(av->getDiamonds() == 150 && av->getFreeDiamonds() == 60);
        CHECK(av->getCumulativePurchasedDiamonds() == 0);
    }
    {
        // Free path, negative (spend): clamped to diamonds when over.
        auto home = makeHome(100, 200);
        LogicDiamondsAddedCommand c;
        c.b_ = true;
        c.i1_ = -30;
        CHECK(c.execute(home.get(), 0, false) == 0);
        const auto* av = home->getPlayerAvatar();
        CHECK(av->getDiamonds() == 70 && av->getFreeDiamonds() == 70);
    }
    {
        // Free path, negative within budget: diamonds drop, free untouched
        // (the binary only ever clamps free DOWN to diamonds, never
        // subtracts — @0x44d1e4 Ladder).
        auto home = makeHome(100, 50);
        LogicDiamondsAddedCommand c;
        c.b_ = true;
        c.i1_ = -30;
        CHECK(c.execute(home.get(), 0, false) == 0);
        const auto* av = home->getPlayerAvatar();
        CHECK(av->getDiamonds() == 70 && av->getFreeDiamonds() == 50);
    }

    // ChangeAvatarName execute @0x1b40d4 + useDiamonds @0x591f4c.
    {
        // Empty name -> 1, no avatar -> 2.
        LogicChangeAvatarNameCommand c;
        CHECK(c.execute(makeHome(0, 0).get(), 0, false) == 1);
        c.name_ = "x";
        LogicHomeMode empty;
        CHECK(c.execute(&empty, 0, false) == 2);
        CHECK(c.execute(nullptr, 0, false) == 2);
    }
    {
        // Rename charges name length in diamonds, sets the flag.
        auto home = makeHome(100, 50);
        home->getPlayerAvatar()->setName("Old");
        LogicChangeAvatarNameCommand c;
        c.name_ = "NewName"; // 7 chars -> 7 diamonds
        CHECK(c.execute(home.get(), 0, false) == 0);
        const auto* av = home->getPlayerAvatar();
        CHECK(av->name_ == "NewName" && av->getNameSetByUser());
        CHECK(av->getDiamonds() == 93 && av->getFreeDiamonds() == 43);
    }
    {
        // useDiamonds clamps free at zero, never below.
        LogicClientAvatar av;
        av.setDiamonds(10);
        av.setFreeDiamonds(3);
        av.useDiamonds(5);
        CHECK(av.getDiamonds() == 5 && av.getFreeDiamonds() == 0);
    }

    if (failures == 0) std::puts("gameplay: all ok");
    return failures == 0 ? 0 : 1;
}
