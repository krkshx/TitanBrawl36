#pragma once

// NotificationFactory::create @0x5f5d50: int type id -> notification.
// The binary switches on char-range ids ('?'=63 … '^'=94, plus stray id 2);
// most cases build a BaseNotification with a subclass vtable, a few call
// dedicated ctors. Type ids verified via each class's getNotificationType.
// Unmapped ids (2/Donate has no factory case; 83/87 resolve to vtables
// whose classes are still open) return nullptr — callers throw
// pending_reverse with the id.

#include "titan/game/notif/BandNotification.cpp"
#include "titan/game/notif/BaseNotification.cpp"
#include "titan/game/notif/BoxRewardNotification.cpp"
#include "titan/game/notif/BrawlPassAutoCollectSeasonNotification.cpp"
#include "titan/game/notif/BrawlPassPointRewardNotification.cpp"
#include "titan/game/notif/BrawlPassRewardNotification.cpp"
#include "titan/game/notif/ChallengeRewardNotification.cpp"
#include "titan/game/notif/ChallengeSkinRewardNotification.cpp"
#include "titan/game/notif/CoinDoublerRewardNotification.cpp"
#include "titan/game/notif/DonateNotification.cpp"
#include "titan/game/notif/FloaterTextNotification.cpp"
#include "titan/game/notif/FreeTextNotification.cpp"
#include "titan/game/notif/GemRewardNotification.cpp"
#include "titan/game/notif/HeroPowerRewardNotification.cpp"
#include "titan/game/notif/HeroRewardNotification.cpp"
#include "titan/game/notif/IAPDeliveryNotification.cpp"
#include "titan/game/notif/ProLeagueSeasonEndNotification.cpp"
#include "titan/game/notif/QualifyNotification.cpp"
#include "titan/game/notif/RankRewardNotification.cpp"
#include "titan/game/notif/RankedMidSeasonRewardNotification.cpp"
#include "titan/game/notif/RankedSeasonEndNotification.cpp"
#include "titan/game/notif/ResourceRewardNotification.cpp"
#include "titan/game/notif/RevokeNotification.cpp"
#include "titan/game/notif/SkinPurchaseOptionRewardNotification.cpp"
#include "titan/game/notif/SkinRewardNotification.cpp"
#include "titan/game/notif/StarPointsNotification.cpp"
#include "titan/game/notif/StarPointMigrationNotification.cpp"
#include "titan/game/notif/StarPowerRewardNotification.cpp"
#include "titan/game/notif/TicketCompensationNotification.cpp"
#include "titan/game/notif/TicketRewardNotification.cpp"
#include "titan/game/avatar/VanityItemRewardNotification.cpp"

#include <memory>

namespace titan {

inline std::unique_ptr<BaseNotification> createNotificationByType(int type) {
    switch (type) {
        case 63: // '?'
        case 70: // 'F' (getter picks by field +64)
            return std::make_unique<ChallengeRewardNotification>();
        case 64: return std::make_unique<BoxRewardNotification>(); // '@'
        case 65: return std::make_unique<SkinPurchaseOptionRewardNotification>(); // 'A'
        case 66: return std::make_unique<FloaterTextNotification>(); // 'B'
        case 67: return std::make_unique<RankedMidSeasonRewardNotification>(); // 'C'
        case 68: return std::make_unique<RankedSeasonEndNotification>(); // 'D'
        case 69: return std::make_unique<BrawlPassAutoCollectSeasonNotification>(); // 'E'
        case 71: return std::make_unique<BrawlPassPointRewardNotification>(); // 'G'
        case 72: return std::make_unique<VanityItemRewardNotification>(); // 'H'
        case 73: return std::make_unique<BrawlPassRewardNotification>(); // 'I'
        case 74: return std::make_unique<TicketCompensationNotification>(); // 'J'
        case 75: return std::make_unique<ChallengeSkinRewardNotification>(); // 'K'
        case 76: return std::make_unique<QualifyNotification>(); // 'L'
        case 77: return std::make_unique<ProLeagueSeasonEndNotification>(); // 'M'
        case 78: return std::make_unique<RankRewardNotification>(); // 'N'
        case 79: return std::make_unique<StarPointsNotification>(); // 'O'
        case 80: return std::make_unique<StarPointMigrationNotification>(); // 'P'
        case 81: return std::make_unique<FreeTextNotification>(); // 'Q'
        case 82: return std::make_unique<BandNotification>(); // 'R'
        case 84: return std::make_unique<StarPowerRewardNotification>(); // 'T'
        case 85: return std::make_unique<RevokeNotification>(); // 'U'
        case 86: return std::make_unique<IAPDeliveryNotification>(); // 'V'
        case 88: return std::make_unique<CoinDoublerRewardNotification>(); // 'X'
        case 89: return std::make_unique<GemRewardNotification>(); // 'Y'
        case 90: return std::make_unique<ResourceRewardNotification>(); // 'Z'
        case 91: return std::make_unique<TicketRewardNotification>(); // '['
        case 92: return std::make_unique<HeroPowerRewardNotification>(); // '\'
        case 93: return std::make_unique<HeroRewardNotification>(); // ']'
        case 94: return std::make_unique<SkinRewardNotification>(); // '^'
        default: return nullptr;
    }
}

} // namespace titan
