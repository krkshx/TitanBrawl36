// Self-check for command execute() semantics (gameplay wave).

#include "titan/commands/LogicChangeAvatarNameCommand.hpp"
#include "titan/commands/LogicDeleteNotificationCommand.hpp"
#include "titan/commands/LogicDiamondsAddedCommand.hpp"
#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicClientHome.hpp"
#include "titan/game/LogicData.hpp"
#include "titan/game/LogicHomeMode.hpp"
#include "titan/game/NotificationFactory.hpp"

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
    // DeleteNotification execute @0x961cc4 (+ removeNotification @0x8f73e4).
    {
        // Missing entry -> warning, returns 0.
        auto home = makeHome(0, 0);
        home->setHome(std::make_unique<LogicClientHome>());
        LogicDeleteNotificationCommand c;
        c.v1_ = 81;
        CHECK(c.execute(home.get(), 0, false) == 0);
        CHECK(home->getHome()->notifications_.empty());
    }
    {
        // Present entry (matched by base +8 int) is removed and returned.
        auto home = makeHome(0, 0);
        home->setHome(std::make_unique<LogicClientHome>());
        auto n = createNotificationByType(81);
        n->v8_ = 81;
        home->getHome()->notifications_.emplace_back(81, std::move(n));
        auto other = createNotificationByType(81);
        other->v8_ = 82;
        home->getHome()->notifications_.emplace_back(81, std::move(other));
        LogicDeleteNotificationCommand c;
        c.v1_ = 82;
        CHECK(c.execute(home.get(), 0, false) == 0);
        const auto& left = home->getHome()->notifications_;
        CHECK(left.size() == 1 && left[0].second->v8_ == 81);
    }
    {
        // No home -> safe no-op.
        LogicDeleteNotificationCommand c;
        LogicHomeMode empty;
        CHECK(c.execute(&empty, 0, false) == 0);
    }
    // Commodity economy (@0x32922c/@0x7c7bc8/@0x5c3590).
    {
        DataTables dt; // empty tables: facades still construct, reads miss
        (void)dt;
        LogicClientAvatar av;
        LogicData gold(nullptr, 5, 0);
        CHECK(av.getCommodityCount(0, gold) == 0);
        av.setCommodityCount(0, gold, 100);
        CHECK(av.getCommodityCount(0, gold) == 100);
        av.setCommodityCount(0, gold, 250);
        CHECK(av.getCommodityCount(0, gold) == 250);
        LogicData other(nullptr, 5, 1);
        CHECK(av.getCommodityCount(0, other) == 0);
        // Delta helper: add, cap (resources class 5 needs tables -> skip
        // cap here, plain add), zero-delta no-op.
        CHECK(av.commodityCountChangeHelper(0, other, 0) == 0);
        CHECK(av.commodityCountChangeHelper(0, other, 30) == 30);
        CHECK(av.getCommodityCount(0, other) == 30);
        CHECK(av.commodityCountChangeHelper(0, other, -10) == -10);
        CHECK(av.getCommodityCount(0, other) == 20);
        // Bad slot: loud error, safe zero.
        CHECK(av.getCommodityCount(9, gold) == 0);
        CHECK(av.commodityCountChangeHelper(9, gold, 5) == 0);
    }

    if (failures == 0) std::puts("gameplay: all ok");
    return failures == 0 ? 0 : 1;
}
