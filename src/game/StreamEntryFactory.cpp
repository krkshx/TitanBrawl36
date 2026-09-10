// Stream entry factories (see StreamEntryFactory.hpp).

#include "titan/game/StreamEntryFactory.hpp"

#include "titan/game/AllianceEventStreamEntry.hpp"
#include "titan/game/AvatarStreamEntry.hpp"
#include "titan/game/ChatStreamEntry.hpp"
#include "titan/game/JoinRequestAllianceStreamEntry.hpp"
#include "titan/game/StreamEntry.hpp"
#include "titan/messages/pending/DeviceLinkedStreamEntry.hpp"
#include "titan/messages/pending/DonationReceivedStreamEntry.hpp"
#include "titan/messages/pending/AllianceInvitationAvatarStreamEntry.hpp"
#include "titan/messages/pending/AllianceKickOutStreamEntry.hpp"
#include "titan/messages/pending/AllianceMailAvatarStreamEntry.hpp"
#include "titan/messages/pending/BattleReportStreamEntry.hpp"
#include "titan/messages/pending/JoinAllianceResponseAvatarStreamEntry.hpp"
#include "titan/messages/pending/MessageDataStreamEntry.hpp"
#include "titan/messages/pending/QuickChatStreamEntry.hpp"
#include "titan/messages/pending/ReplayStreamEntry.hpp"
#include "titan/messages/pending/TeamCreatedStreamEntry.hpp"

namespace titan {

std::unique_ptr<StreamEntry> createAllianceStreamEntry(int type) {
    switch (type) {
        case 2: return std::make_unique<ChatStreamEntry>();
        case 3: return std::make_unique<JoinRequestAllianceStreamEntry>();
        case 4: return std::make_unique<AllianceEventStreamEntry>();
        case 5: return std::make_unique<ReplayStreamEntry>();
        case 6: return std::make_unique<MessageDataStreamEntry>();
        case 7: return nullptr;
        case 8: return std::make_unique<QuickChatStreamEntry>();
        case 77: return std::make_unique<TeamCreatedStreamEntry>();
        default: return nullptr;
    }
}

std::unique_ptr<AvatarStreamEntry> createAvatarStreamEntry(int type) {
    switch (type) {
        case 1:
        case 2: return std::make_unique<BattleReportStreamEntry>();
        case 3: return std::make_unique<JoinAllianceResponseAvatarStreamEntry>();
        case 4: return std::make_unique<AllianceInvitationAvatarStreamEntry>();
        case 5: return std::make_unique<AllianceKickOutStreamEntry>();
        case 6: return std::make_unique<AllianceMailAvatarStreamEntry>();
        case 7: return std::make_unique<DonationReceivedStreamEntry>();
        case 9: return std::make_unique<DeviceLinkedStreamEntry>();
        default: return nullptr;
    }
}

} // namespace titan
