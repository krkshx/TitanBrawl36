#pragma once

// Stream entry factories — reversed from libg_decrypted.so (ARM64).
//
// StreamEntryFactory::createStreamEntryByType @0x90d8cc (alliance scope):
//   2 ChatStreamEntry, 3 JoinRequestAllianceStreamEntry,
//   4 AllianceEventStreamEntry, 5 ReplayStreamEntry, 6 MessageDataStreamEntry,
//   7 null, 8 QuickChatStreamEntry, 77 TeamCreatedStreamEntry.
// AvatarStreamEntryFactory::createStreamEntryByType @0x7f9b50:
//   1-2 BattleReportStreamEntry (dynamic type field +44),
//   3 JoinAllianceResponseAvatarStreamEntry, 4 AllianceInvitationAvatarStreamEntry,
//   5 AllianceKickOutStreamEntry, 6 AllianceMailAvatarStreamEntry,
//   7 DonationReceivedStreamEntry, 9 DeviceLinkedStreamEntry.
// Unreversed subtypes stay pending (factory returns them; their bodies throw
// on use). Unknown types return nullptr like the binary.

#include "titan/game/stream/AvatarStreamEntry.cpp"
#include "titan/game/stream/StreamEntry.cpp"

#include <memory>

namespace titan {

std::unique_ptr<StreamEntry> createAllianceStreamEntry(int type);
std::unique_ptr<AvatarStreamEntry> createAvatarStreamEntry(int type);

} // namespace titan

// Stream entry factories (see StreamEntryFactory.hpp).


#include "titan/game/alliance/AllianceEventStreamEntry.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"
#include "titan/game/stream/ChatStreamEntry.cpp"
#include "titan/game/alliance/JoinRequestAllianceStreamEntry.cpp"
#include "titan/game/stream/StreamEntry.cpp"
#include "titan/game/stream/DeviceLinkedStreamEntry.cpp"
#include "titan/game/stream/DonationReceivedStreamEntry.cpp"
#include "titan/game/alliance/AllianceInvitationAvatarStreamEntry.cpp"
#include "titan/game/alliance/AllianceKickOutStreamEntry.cpp"
#include "titan/game/alliance/AllianceMailAvatarStreamEntry.cpp"
#include "titan/game/stream/BattleReportStreamEntry.cpp"
#include "titan/game/alliance/JoinAllianceResponseAvatarStreamEntry.cpp"
#include "titan/game/stream/MessageDataStreamEntry.cpp"
#include "titan/game/stream/QuickChatStreamEntry.cpp"
#include "titan/game/stream/ReplayStreamEntry.cpp"
#include "titan/game/stream/TeamCreatedStreamEntry.cpp"

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
        case 2: return std::make_unique<BattleReportStreamEntry>(type);
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
