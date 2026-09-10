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

#include "titan/game/AvatarStreamEntry.hpp"
#include "titan/game/StreamEntry.hpp"

#include <memory>

namespace titan {

std::unique_ptr<StreamEntry> createAllianceStreamEntry(int type);
std::unique_ptr<AvatarStreamEntry> createAvatarStreamEntry(int type);

} // namespace titan
