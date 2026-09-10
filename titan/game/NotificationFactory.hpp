#pragma once

// NotificationFactory::create @0x5f5d50: int type id -> notification.
// The binary switches on char-range ids ('?'=63 … '^'=94, plus stray id 2);
// most cases build a BaseNotification with a subclass vtable, a few call
// dedicated ctors. Type ids verified via each class's getNotificationType.
// Unmapped ids (2/Donate has no factory case; 83/87 resolve to vtables
// whose classes are still open) return nullptr — callers throw
// pending_reverse with the id.

#include "titan/game/BandNotification.hpp"
#include "titan/game/BaseNotification.hpp"
#include "titan/game/BoxRewardNotification.hpp"
#include "titan/game/BrawlPassAutoCollectSeasonNotification.hpp"
#include "titan/game/BrawlPassPointRewardNotification.hpp"
#include "titan/game/BrawlPassRewardNotification.hpp"
#include "titan/game/ChallengeRewardNotification.hpp"
#include "titan/game/ChallengeSkinRewardNotification.hpp"
#include "titan/game/CoinDoublerRewardNotification.hpp"
#include "titan/game/DonateNotification.hpp"
#include "titan/game/FloaterTextNotification.hpp"
#include "titan/game/FreeTextNotification.hpp"
#include "titan/game/GemRewardNotification.hpp"
#include "titan/game/HeroPowerRewardNotification.hpp"
#include "titan/game/HeroRewardNotification.hpp"
#include "titan/game/IAPDeliveryNotification.hpp"
#include "titan/game/ProLeagueSeasonEndNotification.hpp"
#include "titan/game/QualifyNotification.hpp"
#include "titan/game/RankRewardNotification.hpp"
#include "titan/game/RankedMidSeasonRewardNotification.hpp"
#include "titan/game/RankedSeasonEndNotification.hpp"
#include "titan/game/ResourceRewardNotification.hpp"
#include "titan/game/RevokeNotification.hpp"
#include "titan/game/SkinPurchaseOptionRewardNotification.hpp"
#include "titan/game/SkinRewardNotification.hpp"
#include "titan/game/StarPointsNotification.hpp"
#include "titan/game/StarPointMigrationNotification.hpp"
#include "titan/game/StarPowerRewardNotification.hpp"
#include "titan/game/TicketCompensationNotification.hpp"
#include "titan/game/TicketRewardNotification.hpp"
#include "titan/game/VanityItemRewardNotification.hpp"

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
