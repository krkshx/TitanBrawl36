// Generated — do not edit. 324 ids, 335 classes.
#include "titan/messages/KeepAliveMessages.hpp"
#include "titan/messages/MsgBatch00.hpp"
#include "titan/messages/MsgBatch01.hpp"
#include "titan/messages/MsgBatch02.hpp"
#include "titan/messages/MsgBatch03.hpp"
#include "titan/messages/MsgBatch04.hpp"
#include "titan/messages/MsgBatch05.hpp"
#include "titan/messages/MsgBatch06.hpp"
#include "titan/messages/MsgBatch07.hpp"
#include "titan/messages/MsgBatch08.hpp"
#include "titan/messages/MsgBatch09.hpp"
#include "titan/messages/MsgBatch10.hpp"
#include "titan/messages/MsgBatch11.hpp"
#include "titan/messages/MsgBatch12.hpp"
#include "titan/messages/MsgBatch13.hpp"
#include "titan/messages/MsgBatch14.hpp"
#include "titan/messages/ResetAccountMessage.hpp"

namespace titan {

std::unique_ptr<PiranhaMessage> createMessageByType(int type) {
    switch (type) {
        case 10055: return std::make_unique<AskPlayerJWTokenMessage>();
        case 10099: return std::make_unique<ClientCryptoErrorMessage>();
        case 10100: return std::make_unique<ClientHelloMessage>();
        case 10101: return std::make_unique<LoginMessage>();
        case 10102: return std::make_unique<LoginUsingSessionMessage>();
        case 10103: return std::make_unique<CreateAccountMessage>();
        case 10107: return std::make_unique<ClientCapabilitiesMessage>();
        case 10108: return std::make_unique<KeepAliveMessage>();
        case 10109: return std::make_unique<UdpCheckConnectionMessage>();
        case 10110: return std::make_unique<AnalyticEventMessage>();
        case 10111: return std::make_unique<AccountIdentifiersMessage>();
        case 10112: return std::make_unique<AuthenticationCheckMessage>();
        case 10113: return std::make_unique<SetDeviceTokenMessage>();
        case 10116: return std::make_unique<ResetAccountMessage>();
        case 10117: return std::make_unique<ReportUserMessage>();
        case 10118: return std::make_unique<AccountSwitchedMessage>();
        case 10119: return std::make_unique<ReportAllianceStreamMessage>();
        case 10121: return std::make_unique<UnlockAccountMessage>();
        case 10150: return std::make_unique<AppleBillingRequestMessage>();
        case 10151: return std::make_unique<GoogleBillingRequestMessage>();
        case 10152: return std::make_unique<TencentBillingRequestMessage>();
        case 10153: return std::make_unique<CafeBazaarBillingRequestMessage>();
        case 10159: return std::make_unique<KunlunBillingRequestMessage>();
        case 10160: return std::make_unique<BillingCancelledByClientMessage>();
        case 10177: return std::make_unique<ClientInfoMessage>();
        case 10212: return std::make_unique<ChangeAvatarNameMessage>();
        case 10309: return std::make_unique<GetAllianceInviteTokenMessage>();
        case 10321: return std::make_unique<AttributionEventMessage>();
        case 10401: return std::make_unique<CreateGameMessage>();
        case 10501: return std::make_unique<AcceptFriendMessage>();
        case 10502: return std::make_unique<AddFriendMessage>();
        case 10503: return std::make_unique<AskForAddableFriendsMessage>();
        case 10504: return std::make_unique<AskForFriendListMessage>();
        case 10506: return std::make_unique<RemoveFriendMessage>();
        case 10507: return std::make_unique<AddFriendByEmailMessage>();
        case 10509: return std::make_unique<AddFriendByAvatarNameAndCodeMessage>();
        case 10512: return std::make_unique<AskForPlayingGamecenterFriendsMessage>();
        case 10513: return std::make_unique<AskForPlayingFacebookFriendsMessage>();
        case 10514: return std::make_unique<AskForPlayingKakaoFriendsMessage>();
        case 10515: return std::make_unique<AskForPlayingTencentFriendsMessage>();
        case 10516: return std::make_unique<AskForPlayingLineFriendsMessage>();
        case 10517: return std::make_unique<AskForPlayingSupercellFriendsMessage>();
        case 10523: return std::make_unique<YoozooBillingRequestMessage>();
        case 10555: return std::make_unique<ClientInputMessage>();
        case 10576: return std::make_unique<SetBlockFriendRequestsMessage>();
        case 10599: return std::make_unique<AskForFriendSuggestionsMessage>();
        case 10636: return std::make_unique<SCIDBindAccountMessage>();
        case 11736: return std::make_unique<SCIDLogoutAllDevicesMessage>();
        case 12100: return std::make_unique<CreatePlayerMapMessage>();
        case 12101: return std::make_unique<DeletePlayerMapMessage>();
        case 12102: return std::make_unique<GetPlayerMapsMessage>();
        case 12103: return std::make_unique<UpdatePlayerMapMessage>();
        case 12104: return std::make_unique<SubmitPlayerMapMessage>();
        case 12105: return std::make_unique<PublishPlayerMapMessage>();
        case 12106: return std::make_unique<ChangePlayerMapNameMessage>();
        case 12107: return std::make_unique<EnterMapEditorMessage>();
        case 12108: return std::make_unique<GoHomeFromMapEditorMessage>();
        case 12110: return std::make_unique<TeamSetPlayerMapMessage>();
        case 12111: return std::make_unique<SignoffPlayerMapMessage>();
        case 12125: return std::make_unique<ReportPlayerMapMessage>();
        case 12152: return std::make_unique<RankedMatchBanHeroMessage>();
        case 12155: return std::make_unique<RankedMatchPickHeroMessage>();
        case 12157: return std::make_unique<RankedMatchUpdateHeroDataMessage>();
        case 12905: return std::make_unique<GetCurrentBattleReplayDataMessage>();
        case 12998: return std::make_unique<SetCountryMessage>();
        case 13922: return std::make_unique<AcceptTokenFriendMessage>();
        case 14101: return std::make_unique<GoHomeMessage>();
        case 14102: return std::make_unique<EndClientTurnMessage>();
        // collision (see data/factory_notes.csv): StartGameMessage
        case 14103: return std::make_unique<MatchmakeRequestMessage>();
        // collision (see data/factory_notes.csv): StartSpectateMessage
        case 14104: return std::make_unique<StartMissionMessage>();
        case 14105: return std::make_unique<HomeLogicStoppedMessage>();
        case 14106: return std::make_unique<CancelMatchmakingMessage>();
        case 14107: return std::make_unique<StopSpectateMessage>();
        case 14108: return std::make_unique<GoHomeFromSpectateMessage>();
        case 14109: return std::make_unique<GoHomeFromOfflinePractiseMessage>();
        case 14110: return std::make_unique<AskForBattleEndMessage>();
        case 14113: return std::make_unique<GetPlayerProfileMessage>();
        // collision (see data/factory_notes.csv): HomeBattleReplayMessage
        case 14114: return std::make_unique<GetBattleLogMessage>();
        case 14115: return std::make_unique<BattleLogViewReplayMessage>();
        case 14116: return std::make_unique<ViewReplayByStringIdMessage>();
        case 14117: return std::make_unique<RequestMatchCancelMessage>();
        case 14118: return std::make_unique<SinglePlayerMatchRequestMessage>();
        case 14166: return std::make_unique<ChronosEventSeenMessage>();
        case 14177: return std::make_unique<PlayAgainMessage>();
        case 14178: return std::make_unique<DebugCommandMessage>();
        case 14199: return std::make_unique<LookForGameRoomRequestMessage>();
        case 14201: return std::make_unique<BindFacebookAccountMessage>();
        case 14202: return std::make_unique<BindKakaoAccountMessage>();
        case 14203: return std::make_unique<BindLineAccountMessage>();
        case 14211: return std::make_unique<UnbindFacebookAccountMessage>();
        case 14212: return std::make_unique<BindGamecenterAccountMessage>();
        case 14213: return std::make_unique<UnbindKakaoAccountMessage>();
        case 14214: return std::make_unique<UnbindLineAccountMessage>();
        case 14262: return std::make_unique<BindGoogleServiceAccountMessage>();
        // collision (see data/factory_notes.csv): BindYoozooAccountMessage
        case 14266: return std::make_unique<BindTencentAccountMessage>();
        case 14267: return std::make_unique<TencentAntiAddictionInstructionExecutedMessage>();
        case 14268: return std::make_unique<TencentCheckCanPayMessage>();
        case 14277: return std::make_unique<GetSeasonRewardsMessage>();
        case 14299: return std::make_unique<SetAllianceCountryMessage>();
        case 14301: return std::make_unique<CreateAllianceMessage>();
        case 14302: return std::make_unique<AskForAllianceDataMessage>();
        case 14303: return std::make_unique<AskForJoinableAlliancesListMessage>();
        case 14304: return std::make_unique<AskForAllianceStreamMessage>();
        case 14305: return std::make_unique<JoinAllianceMessage>();
        case 14306: return std::make_unique<ChangeAllianceMemberRoleMessage>();
        case 14307: return std::make_unique<KickAllianceMemberMessage>();
        case 14308: return std::make_unique<LeaveAllianceMessage>();
        case 14315: return std::make_unique<ChatToAllianceStreamMessage>();
        case 14316: return std::make_unique<ChangeAllianceSettingsMessage>();
        case 14317: return std::make_unique<RequestJoinAllianceMessage>();
        case 14321: return std::make_unique<RespondToAllianceJoinRequestMessage>();
        case 14322: return std::make_unique<SendAllianceInvitationMessage>();
        case 14323: return std::make_unique<JoinAllianceUsingInvitationMessage>();
        case 14324: return std::make_unique<SearchAlliancesMessage>();
        case 14326: return std::make_unique<SendAllianceInvitationToFriendMessage>();
        case 14330: return std::make_unique<SendAllianceMailMessage>();
        case 14350: return std::make_unique<TeamCreateMessage>();
        case 14351: return std::make_unique<TeamJoinMessage>();
        case 14352: return std::make_unique<TeamKickMessage>();
        case 14353: return std::make_unique<TeamLeaveMessage>();
        case 14354: return std::make_unique<TeamChangeMemberSettingsMessage>();
        case 14355: return std::make_unique<TeamSetMemberReadyMessage>();
        case 14356: return std::make_unique<TeamTogglePractiseMessage>();
        case 14357: return std::make_unique<TeamToggleMemberSideMessage>();
        case 14358: return std::make_unique<TeamSpectateMessage>();
        case 14359: return std::make_unique<TeamChatMessage>();
        case 14360: return std::make_unique<TeamPostAdMessage>();
        case 14361: return std::make_unique<TeamMemberStatusMessage>();
        case 14362: return std::make_unique<TeamSetEventMessage>();
        case 14363: return std::make_unique<TeamSetLocationMessage>();
        case 14364: return std::make_unique<TeamReportChatMessage>();
        case 14365: return std::make_unique<TeamInviteMessage>();
        case 14366: return std::make_unique<PlayerStatusMessage>();
        case 14367: return std::make_unique<TeamClearInviteMessage>();
        case 14368: return std::make_unique<TeamInviteResponseMessage>();
        case 14369: return std::make_unique<TeamPremadeChatMessage>();
        case 14370: return std::make_unique<TeamAllianceMemberInviteMessage>();
        case 14371: return std::make_unique<TeamJoinOrCreateRoomMessage>();
        case 14372: return std::make_unique<TeamToggleSettingsMessage>();
        case 14373: return std::make_unique<TeamBotSlotDisableMessage>();
        case 14403: return std::make_unique<GetLeaderboardMessage>();
        case 14405: return std::make_unique<AskForAvatarStreamMessage>();
        case 14406: return std::make_unique<AskForBattleReplayStreamMessage>();
        case 14418: return std::make_unique<RemoveAvatarStreamEntryMessage>();
        case 14479: return std::make_unique<TeamInvitationResponseMessage>();
        case 14600: return std::make_unique<AvatarNameCheckRequestMessage>();
        case 14700: return std::make_unique<ListBrawlTvChannelsMessage>();
        case 14701: return std::make_unique<TuneBrawlTvChannelMessage>();
        case 14715: return std::make_unique<SendGlobalChatLineMessage>();
        case 14777: return std::make_unique<SetInvitesBlockedMessage>();
        case 14778: return std::make_unique<SetTeamChatMutedMessage>();
        case 14867: return std::make_unique<SetRegionMessage>();
        case 14880: return std::make_unique<TeamRequestJoinCancelMessage>();
        case 14881: return std::make_unique<TeamRequestJoinMessage>();
        case 14882: return std::make_unique<TeamRequestJoinApproveMessage>();
        case 15793: return std::make_unique<GetTokenFriendMessage>();
        case 16000: return std::make_unique<LogicDeviceLinkCodeRequestMessage>();
        case 16001: return std::make_unique<LogicDeviceLinkMenuClosedMessage>();
        case 16002: return std::make_unique<LogicDeviceLinkEnterCodeMessage>();
        case 16003: return std::make_unique<LogicDeviceLinkConfirmYesMessage>();
        case 16939: return std::make_unique<AskApiTokenMessage>();
        case 17000: return std::make_unique<LogicAccountTransferCodeRequestMessage>();
        case 17190: return std::make_unique<JoinAllianceUsingTokenMessage>();
        case 17337: return std::make_unique<UnbotifyReportMessage>();
        case 17338: return std::make_unique<AdjustPackageMessage>();
        case 18686: return std::make_unique<SetSupportedCreatorMessage>();
        case 19001: return std::make_unique<LatencyTestResultMessage>();
        case 19002: return std::make_unique<UdpLatencyTestRequestMessage>();
        case 19003: return std::make_unique<TriggerStartLatencyTestMessage>();
        case 19004: return std::make_unique<RequestLatencyTestStatusMessage>();
        // collision (see data/factory_notes.csv): SetEncryptionMessage
        case 20000: return std::make_unique<ExtendedSetEncryptionMessage>();
        case 20100: return std::make_unique<ServerHelloMessage>();
        // collision (see data/factory_notes.csv): CreateAccountOkMessage, UdpBigMessageFragmentMessage
        case 20101: return std::make_unique<CreateAccountFailedMessage>();
        case 20103: return std::make_unique<LoginFailedMessage>();
        // collision (see data/factory_notes.csv): LoginOkMessage
        case 20104: return std::make_unique<AuthenticationResponseMessage>();
        case 20105: return std::make_unique<FriendListMessage>();
        case 20106: return std::make_unique<FriendListUpdateMessage>();
        case 20107: return std::make_unique<AddableFriendsMessage>();
        case 20108: return std::make_unique<KeepAliveServerMessage>();
        case 20109: return std::make_unique<FriendOnlineStatusMessage>();
        case 20112: return std::make_unique<AddFriendFailedMessage>();
        case 20117: return std::make_unique<ReportUserStatusMessage>();
        case 20118: return std::make_unique<ChatAccountBanStatusMessage>();
        case 20121: return std::make_unique<BillingRequestFailedMessage>();
        case 20132: return std::make_unique<UnlockAccountOkMessage>();
        case 20133: return std::make_unique<UnlockAccountFailedMessage>();
        case 20151: return std::make_unique<AppleBillingProcessedByServerMessage>();
        case 20152: return std::make_unique<GoogleBillingProcessedByServerMessage>();
        case 20153: return std::make_unique<TencentBillingProcessedByServerMessage>();
        case 20154: return std::make_unique<CafeBazaarBillingProcessedByServerMessage>();
        case 20156: return std::make_unique<KunlunBillingProcessedByServerMessage>();
        case 20161: return std::make_unique<ShutdownStartedMessage>();
        case 20171: return std::make_unique<PersonalBreakStartedMessage>();
        case 20173: return std::make_unique<YoozooBillingProcessedByServerMessage>();
        case 20199: return std::make_unique<FriendSuggestionsMessage>();
        case 20205: return std::make_unique<AvatarNameChangeFailedMessage>();
        case 20207: return std::make_unique<AllianceOnlineStatusUpdatedMessage>();
        case 20300: return std::make_unique<AvatarNameCheckResponseMessage>();
        case 20402: return std::make_unique<CreateGameFailedMessage>();
        case 20405: return std::make_unique<MatchMakingStatusMessage>();
        case 20406: return std::make_unique<MatchMakingCancelledMessage>();
        case 20501: return std::make_unique<AcceptFriendFailedMessage>();
        case 20523: return std::make_unique<YoozooOrderAvailableMessage>();
        case 20545: return std::make_unique<YoozooOrderDeliveryFailedMessage>();
        case 20559: return std::make_unique<StartLoadingMessage>();
        case 20801: return std::make_unique<NotificationMessage>();
        case 20931: return std::make_unique<AntiAddictionDataUpdatedMessage>();
        case 22089: return std::make_unique<GetTokenFriendResultMessage>();
        case 22100: return std::make_unique<CreatePlayerMapResponseMessage>();
        case 22101: return std::make_unique<DeletePlayerMapResponseMessage>();
        case 22102: return std::make_unique<PlayerMapsMessage>();
        case 22103: return std::make_unique<UpdatePlayerMapResponseMessage>();
        case 22104: return std::make_unique<SubmitPlayerMapResponseMessage>();
        case 22105: return std::make_unique<PublishPlayerMapResponseMessage>();
        case 22106: return std::make_unique<ChangePlayerMapNameResponseMessage>();
        case 22107: return std::make_unique<PlayerMapInfoUpdatedMessage>();
        case 22109: return std::make_unique<DebugPlayerMapReviewResultOverrideSetMessage>();
        case 22111: return std::make_unique<PlayerMapGreenlightedMessage>();
        case 22125: return std::make_unique<ReportPlayerMapResponseMessage>();
        case 22150: return std::make_unique<RankedMatchStartedMessage>();
        case 22151: return std::make_unique<RankedMatchBanStartedMessage>();
        case 22152: return std::make_unique<RankedMatchBanHeroResponseMessage>();
        case 22153: return std::make_unique<RankedMatchBanEndedMessage>();
        case 22154: return std::make_unique<RankedMatchPickStartedMessage>();
        case 22155: return std::make_unique<RankedMatchPickHeroFailedMessage>();
        case 22156: return std::make_unique<RankedMatchHeroPickedMessage>();
        case 22157: return std::make_unique<RankedMatchHeroDataUpdatedMessage>();
        case 22158: return std::make_unique<RankedMatchFinalPreparationStartedMessage>();
        case 22159: return std::make_unique<RankedMatchTerminatedMessage>();
        case 22202: return std::make_unique<MapPreviewMessage>();
        case 22377: return std::make_unique<GoogleServiceAccountBoundMessage>();
        case 22687: return std::make_unique<GamecenterAccountAlreadyBoundMessage>();
        case 22957: return std::make_unique<PvpMatchmakeNotificationMessage>();
        case 23067: return std::make_unique<SCIDLogoutAllDevicesResultMessage>();
        case 23302: return std::make_unique<GetAllianceInviteTokenResultMessage>();
        case 23456: return std::make_unique<BattleEndMessage>();
        case 23457: return std::make_unique<LobbyInfoMessage>();
        case 23458: return std::make_unique<BattleLogMessage>();
        case 23459: return std::make_unique<BattleLogReplayAvailableMessage>();
        case 23494: return std::make_unique<GoogleServiceAccountAlreadyBoundMessage>();
        case 23774: return std::make_unique<PlayerJWTokenMessage>();
        case 24101: return std::make_unique<OwnHomeDataMessage>();
        case 24104: return std::make_unique<OutOfSyncMessage>();
        case 24105: return std::make_unique<SpectateFailedMessage>();
        case 24106: return std::make_unique<StopHomeLogicMessage>();
        case 24108: return std::make_unique<MatchmakeFailedMessage>();
        case 24109: return std::make_unique<VisionUpdateMessage>();
        case 24111: return std::make_unique<AvailableServerCommandMessage>();
        case 24112: return std::make_unique<UdpConnectionInfoMessage>();
        case 24113: return std::make_unique<PlayerProfileMessage>();
        case 24114: return std::make_unique<HomeBattleReplayDataMessage>();
        case 24115: return std::make_unique<ServerErrorMessage>();
        // collision (see data/factory_notes.csv): HomeBattleReplayFailedMessage
        case 24116: return std::make_unique<DebugNewbieCoopOverrideSetMessage>();
        case 24117: return std::make_unique<HomeBattleReplayViewedMessage>();
        // collision (see data/factory_notes.csv): SeasonRewardsMessage
        case 24123: return std::make_unique<DailyEventsMessage>();
        case 24124: return std::make_unique<TeamMessage>();
        case 24125: return std::make_unique<TeamLeftMessage>();
        case 24129: return std::make_unique<TeamErrorMessage>();
        case 24130: return std::make_unique<TeamGameStartingMessage>();
        case 24131: return std::make_unique<TeamStreamMessage>();
        case 24177: return std::make_unique<SetRegionResponseMessage>();
        case 24178: return std::make_unique<SetCountryResponseMessage>();
        case 24201: return std::make_unique<FacebookAccountBoundMessage>();
        case 24202: return std::make_unique<FacebookAccountAlreadyBoundMessage>();
        case 24203: return std::make_unique<KakaoAccountBoundMessage>();
        case 24204: return std::make_unique<KakaoAccountAlreadyBoundMessage>();
        case 24205: return std::make_unique<LineAccountAlreadyBoundMessage>();
        case 24206: return std::make_unique<LineAccountBoundMessage>();
        case 24214: return std::make_unique<FacebookAccountUnboundMessage>();
        case 24215: return std::make_unique<KakaoAccountUnboundMessage>();
        case 24216: return std::make_unique<LineAccountUnboundMessage>();
        case 24220: return std::make_unique<TencentAccountBoundMessage>();
        case 24221: return std::make_unique<TencentAccountAlreadyBoundMessage>();
        case 24222: return std::make_unique<TencentAntiAddictionInstructionMessage>();
        case 24223: return std::make_unique<TencentCheckCanPayResponseMessage>();
        case 24301: return std::make_unique<AllianceDataMessage>();
        case 24304: return std::make_unique<JoinableAllianceListMessage>();
        case 24308: return std::make_unique<AllianceMemberMessage>();
        case 24309: return std::make_unique<AllianceMemberRemovedMessage>();
        case 24310: return std::make_unique<AllianceListMessage>();
        case 24311: return std::make_unique<AllianceStreamMessage>();
        case 24312: return std::make_unique<AllianceStreamEntryMessage>();
        case 24313: return std::make_unique<ChangeAllianceSettingsOkMessage>();
        case 24318: return std::make_unique<AllianceStreamEntryRemovedMessage>();
        case 24319: return std::make_unique<TeamStreamEntryRemovedMessage>();
        case 24321: return std::make_unique<AllianceInvitationSendFailedMessage>();
        case 24333: return std::make_unique<AllianceResponseMessage>();
        case 24364: return std::make_unique<AllianceTeamsMessage>();
        case 24365: return std::make_unique<AllianceTeamRemovedMessage>();
        case 24399: return std::make_unique<MyAllianceMessage>();
        case 24403: return std::make_unique<LeaderboardMessage>();
        case 24411: return std::make_unique<AvatarStreamMessage>();
        case 24412: return std::make_unique<AvatarStreamEntryMessage>();
        case 24413: return std::make_unique<BattleReportStreamMessage>();
        case 24418: return std::make_unique<AvatarStreamEntryRemovedMessage>();
        case 24555: return std::make_unique<FriendOnlineStatusEntryMessage>();
        case 24582: return std::make_unique<TeamInviteStatusMessage>();
        case 24589: return std::make_unique<TeamInvitationMessage>();
        case 24700: return std::make_unique<BrawlTvChannelListMessage>();
        case 24701: return std::make_unique<BrawlTvChannelNextUpMessage>();
        case 24715: return std::make_unique<GlobalChatLineMessage>();
        case 24758: return std::make_unique<ApiTokenMessage>();
        case 24776: return std::make_unique<AllianceWarMessage>();
        case 24777: return std::make_unique<PlayAgainStatusMessage>();
        case 25165: return std::make_unique<SCIDAccountBoundMessage>();
        // collision (see data/factory_notes.csv): TitanDisconnectedMessage
        case 25892: return std::make_unique<DisconnectedMessage>();
        case 26002: return std::make_unique<LogicDeviceLinkCodeResponseMessage>();
        case 26003: return std::make_unique<LogicDeviceLinkNewDeviceLinkedMessage>();
        case 26004: return std::make_unique<LogicDeviceLinkCodeDeactivatedMessage>();
        case 26005: return std::make_unique<LogicDeviceLinkResponseMessage>();
        case 26007: return std::make_unique<LogicDeviceLinkDoneMessage>();
        case 26008: return std::make_unique<LogicDeviceLinkErrorMessage>();
        case 26085: return std::make_unique<GamecenterAccountBoundMessage>();
        case 27002: return std::make_unique<LogicAccountTransferCodeResponseMessage>();
        case 28363: return std::make_unique<BuyBundleBillingPackResponseMessage>();
        case 28686: return std::make_unique<SetSupportedCreatorResponseMessage>();
        case 28689: return std::make_unique<SCIDAccountAlreadyBoundMessage>();
        case 29001: return std::make_unique<StartLatencyTestRequestMessage>();
        case 29002: return std::make_unique<UdpLatencyTestResponseMessage>();
        case 29003: return std::make_unique<LatencyTestStatusMessage>();
        case 29900: return std::make_unique<SupercellIdNotificationMessage>();
        case 29997: return std::make_unique<CryptoErrorMessage>();
        case 30000: return std::make_unique<AttributionMessage>();
        case 40000: return std::make_unique<AdUpdateConversionValueMessage>();
        default: return nullptr;
    }
}

} // namespace titan
