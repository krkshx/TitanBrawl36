// Generated — do not edit.
#include "titan/gen/LogicCommands.hpp"
#include "titan/commands/LogicAddNotificationCommand.hpp"
#include "titan/commands/LogicBrawlPassAutoCollectWarningSeenCommand.hpp"
#include "titan/commands/LogicBrawlPassSeasonChangedCommand.hpp"
#include "titan/commands/LogicBrawlPassUnlockedCommand.hpp"
#include "titan/commands/LogicCancelPurchaseOfferCommand.hpp"
#include "titan/commands/LogicChangeAvatarNameCommand.hpp"
#include "titan/commands/LogicChangeControlModeCommand.hpp"
#include "titan/commands/LogicChangeResourcesCommand.hpp"
#include "titan/commands/LogicClaimAdRewardCommand.hpp"
#include "titan/commands/LogicClaimDailyRewardCommand.hpp"
#include "titan/commands/LogicClaimRankUpRewardCommand.hpp"
#include "titan/commands/LogicClaimTailRewardCommand.hpp"
#include "titan/commands/LogicClearESportsHubNotificationCommand.hpp"
#include "titan/commands/LogicClearShopTickersCommand.hpp"
#include "titan/commands/LogicCommand.hpp"
#include "titan/commands/LogicCooldownAddedCommand.hpp"
#include "titan/commands/LogicCooldownExpiredCommand.hpp"
#include "titan/commands/LogicDayChangedCommand.hpp"
#include "titan/commands/LogicDeleteNotificationCommand.hpp"
#include "titan/commands/LogicDiamondsAddedCommand.hpp"
#include "titan/commands/LogicGatchaCommand.hpp"
#include "titan/commands/LogicGemNameChangeStateChangedCommand.hpp"
#include "titan/commands/LogicGiveDeliveryItemsCommand.hpp"
#include "titan/commands/LogicHeroSeenCommand.hpp"
#include "titan/commands/LogicHeroWinQuestsChangedCommand.hpp"
#include "titan/commands/LogicIAPChangedCommand.hpp"
#include "titan/commands/LogicInviteBlockingChangedCommand.hpp"
#include "titan/commands/LogicItemSeenCommand.hpp"
#include "titan/commands/LogicKeyPoolChangedCommand.hpp"
#include "titan/commands/LogicLevelUpCommand.hpp"
#include "titan/commands/LogicOffersChangedCommand.hpp"
#include "titan/commands/LogicPlayerDataChangedCommand.hpp"
#include "titan/commands/LogicProLeagueSeasonChangedCommand.hpp"
#include "titan/commands/LogicPurchaseBrawlPassCommand.hpp"
#include "titan/commands/LogicPurchaseBrawlPassProgressCommand.hpp"
#include "titan/commands/LogicPurchaseChallengeLivesCommand.hpp"
#include "titan/commands/LogicPurchaseDoubleCoinsCommand.hpp"
#include "titan/commands/LogicPurchaseHeroLvlUpMaterialCommand.hpp"
#include "titan/commands/LogicPurchaseOfferCommand.hpp"
#include "titan/commands/LogicQuestsSeenCommand.hpp"
#include "titan/commands/LogicRankedSeasonChangedCommand.hpp"
#include "titan/commands/LogicSelectCharacterCommand.hpp"
#include "titan/commands/LogicSelectEmoteCommand.hpp"
#include "titan/commands/LogicSelectGroupSkinCommand.hpp"
#include "titan/commands/LogicSelectSkinCommand.hpp"
#include "titan/commands/LogicSelectStarPowerCommand.hpp"
#include "titan/commands/LogicServerCommand.hpp"
#include "titan/commands/LogicSetESportsHubNotificationCommand.hpp"
#include "titan/commands/LogicSetPlayerAgeCommand.hpp"
#include "titan/commands/LogicSetPlayerNameColorCommand.hpp"
#include "titan/commands/LogicSetPlayerThumbnailCommand.hpp"
#include "titan/commands/LogicSetSupportedCreatorCommand.hpp"
#include "titan/commands/LogicTeamChatMuteStateChangedCommand.hpp"
#include "titan/commands/LogicToggleInGameHintsCommand.hpp"
#include "titan/commands/LogicTransactionsRevokedCommand.hpp"
#include "titan/commands/LogicUnlockFreeSkinsCommand.hpp"
#include "titan/commands/LogicUnlockSkinCommand.hpp"
#include "titan/commands/LogicVanityItemSeenCommand.hpp"
#include "titan/commands/LogicVideoStartedCommand.hpp"
#include "titan/commands/LogicViewInboxNotificationCommand.hpp"

namespace titan {

std::unique_ptr<LogicCommand> createCommandByType(int type) {
    switch (type) {
        case 201: return std::make_unique<LogicChangeAvatarNameCommand>();
        case 202: return std::make_unique<LogicDiamondsAddedCommand>();
        case 203: return std::make_unique<LogicGiveDeliveryItemsCommand>();
        case 204: return std::make_unique<LogicDayChangedCommand>();
        case 205: return std::make_unique<LogicServerCommand>();
        case 206: return std::make_unique<LogicAddNotificationCommand>();
        case 207: return std::make_unique<LogicChangeResourcesCommand>();
        case 208: return std::make_unique<LogicTransactionsRevokedCommand>();
        case 209: return std::make_unique<LogicKeyPoolChangedCommand>();
        case 210: return std::make_unique<LogicIAPChangedCommand>();
        case 211: return std::make_unique<LogicOffersChangedCommand>();
        case 212: return std::make_unique<LogicPlayerDataChangedCommand>();
        case 213: return std::make_unique<LogicInviteBlockingChangedCommand>();
        case 214: return std::make_unique<LogicGemNameChangeStateChangedCommand>();
        case 215: return std::make_unique<LogicSetSupportedCreatorCommand>();
        case 216: return std::make_unique<LogicCooldownExpiredCommand>();
        case 217: return std::make_unique<LogicProLeagueSeasonChangedCommand>();
        case 218: return std::make_unique<LogicBrawlPassSeasonChangedCommand>();
        case 219: return std::make_unique<LogicBrawlPassUnlockedCommand>();
        case 220: return std::make_unique<LogicHeroWinQuestsChangedCommand>();
        case 221: return std::make_unique<LogicTeamChatMuteStateChangedCommand>();
        case 222: return std::make_unique<LogicRankedSeasonChangedCommand>();
        case 223: return std::make_unique<LogicCooldownAddedCommand>();
        case 224: return std::make_unique<LogicSetESportsHubNotificationCommand>();
        case 500: return std::make_unique<LogicGatchaCommand>();
        case 503: return std::make_unique<LogicClaimDailyRewardCommand>();
        case 505: return std::make_unique<LogicSetPlayerThumbnailCommand>();
        case 506: return std::make_unique<LogicSelectSkinCommand>();
        case 507: return std::make_unique<LogicUnlockSkinCommand>();
        case 508: return std::make_unique<LogicChangeControlModeCommand>();
        case 509: return std::make_unique<LogicPurchaseDoubleCoinsCommand>();
        case 511: return std::make_unique<LogicCommandBase511>();
        case 512: return std::make_unique<LogicToggleInGameHintsCommand>();
        case 514: return std::make_unique<LogicDeleteNotificationCommand>();
        case 515: return std::make_unique<LogicClearShopTickersCommand>();
        case 517: return std::make_unique<LogicClaimRankUpRewardCommand>();
        case 519: return std::make_unique<LogicPurchaseOfferCommand>();
        case 520: return std::make_unique<LogicLevelUpCommand>();
        case 521: return std::make_unique<LogicPurchaseHeroLvlUpMaterialCommand>();
        case 522: return std::make_unique<LogicHeroSeenCommand>();
        case 523: return std::make_unique<LogicClaimAdRewardCommand>();
        case 524: return std::make_unique<LogicVideoStartedCommand>();
        case 525: return std::make_unique<LogicSelectCharacterCommand>();
        case 526: return std::make_unique<LogicUnlockFreeSkinsCommand>();
        case 527: return std::make_unique<LogicSetPlayerNameColorCommand>();
        case 528: return std::make_unique<LogicViewInboxNotificationCommand>();
        case 529: return std::make_unique<LogicSelectStarPowerCommand>();
        case 530: return std::make_unique<LogicSetPlayerAgeCommand>();
        case 531: return std::make_unique<LogicCancelPurchaseOfferCommand>();
        case 532: return std::make_unique<LogicItemSeenCommand>();
        case 533: return std::make_unique<LogicQuestsSeenCommand>();
        case 534: return std::make_unique<LogicPurchaseBrawlPassCommand>();
        case 535: return std::make_unique<LogicClaimTailRewardCommand>();
        case 536: return std::make_unique<LogicPurchaseBrawlPassProgressCommand>();
        case 537: return std::make_unique<LogicVanityItemSeenCommand>();
        case 538: return std::make_unique<LogicSelectEmoteCommand>();
        case 539: return std::make_unique<LogicBrawlPassAutoCollectWarningSeenCommand>();
        case 540: return std::make_unique<LogicPurchaseChallengeLivesCommand>();
        case 541: return std::make_unique<LogicClearESportsHubNotificationCommand>();
        case 542: return std::make_unique<LogicSelectGroupSkinCommand>();
        default: return nullptr;
    }
}

void encodeCommandList(ByteStream& s,
                       const std::vector<std::unique_ptr<LogicCommand>>& cmds) {
    s.writeVInt(static_cast<i32>(cmds.size()));
    for (const auto& c : cmds) encodeSingleCommand(s, c);
}

void decodeCommandList(ByteStream& s,
                       std::vector<std::unique_ptr<LogicCommand>>& cmds) {
    const i32 n = s.readVInt();
    cmds.clear();
    for (i32 i = 0; i < n; ++i) cmds.push_back(decodeSingleCommand(s));
}

void encodeSingleCommand(ByteStream& s, const std::unique_ptr<LogicCommand>& c) {
    if (!c) throw std::runtime_error("null command");
    s.writeVInt(c->getCommandType());
    c->encode(s);
}

std::unique_ptr<LogicCommand> decodeSingleCommand(ByteStream& s) {
    auto c = createCommandByType(s.readVInt());
    if (!c) throw std::runtime_error("unknown command type");
    c->decode(s);
    return c;
}

} // namespace titan
