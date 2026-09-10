// GatchaDrop::doDrop @0x4083c4.

#include "titan/game/GatchaDrop.hpp"

#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicDailyData.hpp"
#include "titan/game/LogicHomeMode.hpp"

namespace titan {

void GatchaDrop::doDrop(LogicHomeMode* home, bool flag) {
    LogicClientAvatar* avatar = home ? home->getPlayerAvatar() : nullptr;
    if (!avatar) return;
    switch (v0_) {
        case 2: { // playerData +172 += amount
            LogicClientHome* h = home->getHome();
            if (!h || !h->daily_) return;
            h->daily_->tail43_[0] += v4_;
            return;
        }
        case 3: { // playerData +280 += amount
            LogicClientHome* h = home->getHome();
            if (!h || !h->daily_) return;
            h->daily_->v70_ += v4_;
            return;
        }
        case 8: { // diamonds (paid iff flag; listener arg dropped)
            if (flag) {
                avatar->addPaidDiamonds(v4_);
            } else {
                avatar->addFreeDiamonds(v4_);
            }
            return;
        }
        case 1: // unlockHero (needs card tables)
        case 4: // setItem + addNewItem (needs card tables)
        case 6: // increaseHeroPower (needs hero tables)
        case 7: // material via DataTables global (unresolvable statically)
        case 9: // addUnlockedSkin (needs skin tables)
        case 0xB: // vanity add (needs emote tables)
        case 0xC: // star-points material (needs DataTables global)
            throw pending_reverse("GatchaDrop::doDrop: type needs data tables");
        default:
            return; // binary default: no-op
    }
}

} // namespace titan
