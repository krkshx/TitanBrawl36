// Self-check for command execute() semantics (gameplay wave).

#include "titan/commands/LogicChangeAvatarNameCommand.hpp"
#include "titan/commands/LogicDeleteNotificationCommand.hpp"
#include "titan/commands/LogicDiamondsAddedCommand.hpp"
#include "titan/commands/LogicPurchaseDoubleCoinsCommand.hpp"
#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicClientHome.hpp"
#include "titan/game/LogicConfData.hpp"
#include "titan/game/LogicDailyData.hpp"
#include "titan/game/LogicData.hpp"
#include "titan/game/GatchaDrop.hpp"
#include "titan/game/IntValueEntry.hpp"
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
    // Hero levels (@0x2b9b74/@0x8a2ae0) + level-up cost (@0x708b44).
    {
        LogicClientAvatar av;
        LogicData shelly(nullptr, 16, 0);
        CHECK(av.getHeroLevel(shelly) == 1);
        av.setHeroLevel(shelly, 5);
        CHECK(av.getHeroLevel(shelly) == 5);
        LogicData colt(nullptr, 16, 1);
        CHECK(av.getHeroLevel(colt) == 1);
    }
    {
        LogicConfData conf;
        CHECK(conf.getLevelUpCost(1) == 0);
        conf.ints17_ = {10, 20, 30};
        CHECK(conf.getLevelUpCost(1) == 10);
        CHECK(conf.getLevelUpCost(3) == 30);
        CHECK(conf.getLevelUpCost(4) == 0);
        CHECK(conf.getLevelUpCost(0) == 0);
    }
    // Diamond helpers (@0x61f7c0/@0x73f334).
    {
        LogicClientAvatar av;
        av.setDiamonds(100);
        av.setFreeDiamonds(10);
        av.addFreeDiamonds(0);
        CHECK(av.getDiamonds() == 100 && av.getFreeDiamonds() == 10);
        av.addFreeDiamonds(25);
        CHECK(av.getDiamonds() == 125 && av.getFreeDiamonds() == 35);
        av.addPaidDiamonds(0);
        CHECK(av.getCumulativePurchasedDiamonds() == 0);
        av.addPaidDiamonds(40);
        CHECK(av.getDiamonds() == 165 && av.getFreeDiamonds() == 35);
        CHECK(av.getCumulativePurchasedDiamonds() == 40);
    }
    // HomeMode tables wiring (gold = resources row 1).
    {
        LogicHomeMode home;
        CHECK(home.dataTables() == nullptr);
        DataTables dt;
        home.setDataTables(&dt);
        CHECK(home.dataTables() == &dt);
        const LogicData gold = home.goldData();
        CHECK(gold.classId() == 5 && gold.row() == 1);
    }
    // GatchaDrop::doDrop @0x4083c4 (cases 2/3/8 live).
    {
        auto home = makeHome(100, 10);
        home->setHome(std::make_unique<LogicClientHome>());
        home->getHome()->daily_ = std::make_unique<LogicDailyData>();
        // Case 3: playerData +280 grows.
        GatchaDrop d3;
        d3.v0_ = 3;
        d3.v4_ = 25;
        d3.doDrop(home.get(), false);
        CHECK(home->getHome()->daily_->v70_ == 25);
        // Case 2: playerData +172 grows.
        GatchaDrop d2;
        d2.v0_ = 2;
        d2.v4_ = 7;
        d2.doDrop(home.get(), false);
        CHECK(home->getHome()->daily_->tail43_[0] == 7);
        // Case 8 free: both wallets grow.
        GatchaDrop d8;
        d8.v0_ = 8;
        d8.v4_ = 5;
        d8.doDrop(home.get(), false);
        CHECK(home->getPlayerAvatar()->getDiamonds() == 105);
        CHECK(home->getPlayerAvatar()->getFreeDiamonds() == 15);
        // Case 8 paid: diamonds + cumulative, free untouched.
        d8.doDrop(home.get(), true);
        CHECK(home->getPlayerAvatar()->getDiamonds() == 110);
        CHECK(home->getPlayerAvatar()->getFreeDiamonds() == 15);
        CHECK(home->getPlayerAvatar()->getCumulativePurchasedDiamonds() == 5);
        // Unknown type: silent no-op like the binary default.
        GatchaDrop dx;
        dx.v0_ = 999;
        dx.v4_ = 1000;
        dx.doDrop(home.get(), false);
        CHECK(home->getPlayerAvatar()->getDiamonds() == 110);
        // Table-backed types throw loudly until their tables land.
        GatchaDrop d1;
        d1.v0_ = 1;
        bool threw = false;
        try {
            d1.doDrop(home.get(), false);
        } catch (const pending_reverse&) {
            threw = true;
        }
        CHECK(threw);
    }
    // ConfData int values (@0x4698d8/@0x51a1f0/@0x999c84).
    {
        LogicConfData conf;
        CHECK(conf.getIntValue(10006) == 40); // default table
        CHECK(conf.getIntValue(10007) == 1000);
        CHECK(conf.getIntValue(10022) == 604800);
        CHECK(conf.getIntValue(99999) == 0);
        CHECK(conf.getIntValueEntry(10006) == nullptr);
        auto e = std::make_unique<IntValueEntry>();
        e->a_ = 10006;
        e->b_ = 42;
        conf.intValues_.push_back(std::move(e));
        CHECK(conf.getIntValue(10006) == 42); // entry beats default
        CHECK(conf.getIntValueEntry(10006)->b_ == 42);
        CHECK(conf.getIntValue(10006, 7) == 42); // entry beats fallback
        CHECK(conf.getIntValue(99999, 7) == 7); // fallback (@0x26187c)
    }
    // PurchaseDoubleCoins execute @0x92b034.
    {
        // No home/avatar -> silent 0.
        LogicPurchaseDoubleCoinsCommand c;
        LogicHomeMode empty;
        CHECK(c.execute(&empty, 0, false) == 0);
        // Broke (default price 10006 = 40) -> silent 0.
        auto home = makeHome(30, 0);
        home->setHome(std::make_unique<LogicClientHome>());
        home->getHome()->daily_ = std::make_unique<LogicDailyData>();
        home->getHome()->conf_ = std::make_unique<LogicConfData>();
        CHECK(c.execute(home.get(), 0, false) == 0);
        CHECK(home->getPlayerAvatar()->getDiamonds() == 30);
        // Rich: charged + daily bonus booked (default 10007 = 1000).
        home->getPlayerAvatar()->setDiamonds(500);
        CHECK(c.execute(home.get(), 0, false) == 0);
        CHECK(home->getPlayerAvatar()->getDiamonds() == 460);
        CHECK(home->getHome()->daily_->tail43_[0] == 1000); // default 10007
    }

    if (failures == 0) std::puts("gameplay: all ok");
    return failures == 0 ? 0 : 1;
}
