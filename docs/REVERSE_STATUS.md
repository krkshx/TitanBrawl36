# Reverse status — every function accounted for

Inventory: **37,064** functions (`data/ida_shard_00..14.csv`), all with
unique addresses, see `docs/IDA_BASELINE.md`.
Registry (compiled in): **1861 reimplemented, 9717 third-party, 25486 pending**
(`titan_registry_test` is the source of truth — update these numbers when it moves).

| Bucket | Count | Handling |
|---|---|---|
| Third-party (`ThirdPartyExternal`) | 9717 | system libs, see `docs/THIRDPARTY.md` |
| Game code reimplemented | 1861 fns | `src/` + tests |
| Game code pending | 25486 | `FunctionRegistry` status `Pending` |

## Done

- `ChecksumEncoder` — writeBoolean/writeVInt/writeInt/writeStringLength/
  writeStringReference (`0x534690 0x5c50e0 0x8a558c 0x6ed97c 0x69a564`)
- `ByteStream` — write path (`0x3294c0 0x3f3bd0 0x87bc64 0x298f64
  0x5174d0 0x61bd08 0x608f14`) + read path (`0x23bc10 0x191dc4 0x356c40
  0x28f62c`) + byte/short/longlong + `isAtEnd` (`0x7625a8`) + raw-bytes
- `PiranhaMessage` — encode guard no-op (`0x8f2e70`)
- `LogicLong` (`0x90c8d8`), `DataReference` helper (`0x3a5dec`)
- **All 326 `*Message` protocol surfaces** (`MsgBatch00..13`,
  `ResetAccountMessage`, `KeepAlive*`): encode/decode bit-exact for
  scalars; nested entries pending via `NestedEntry` (empty arrays work).
  Includes `LoginOkMessage` isAtEnd-guarded tail, `BattleEndMessage`,
  `OwnHomeDataMessage` shells, team/ranked/billing/UDP families.
- Send path (`Messaging::encryptAndWrite @0x93221c`): `net::encodeSendFrame`
  (encode → 10100/10101 plaintext bypass → session encrypt → 7-byte header
  over final length; asio socket write stays platform code). Covered by
  `test_factory`.
- Nested entries wave 1: `LogicClientAvatar` (`game/`), `AllianceHeaderEntry`,
  `AllianceFullEntry`, `AllianceMemberEntry`, `LogicCommand` family
  (59 per-class commands + `createCommandByType @0x7c41d8`),
  `LogicCompressedString` (compress `@0x93c13c`, decompress `@0x43760c`).
- Nested entries wave 2 (started): `LogicDailyData` (`@0x6985dc/@0x4356a4`,
  incl. the skin-map key/count aliasing quirk) + leaves `ForcedDrops`
  (`@0x664128`), `TimedOffer` (`@0x354c54`), `IntValueEntry` (`@0x6b0790`),
  `CooldownEntry` (`@0x5402a0`), `LogicConfData` (`@0x653b94/@0x6f6508`) +
  leaves `EventSlot` (`@0x587bb8`), `ReleaseEntry` (`@0x3ef33c`),
  `TimedIntValueEntry` (`@0x6e4ee8`), `CustomEvent` (`@0x911588`),
  `QuestData` (`@0x467740`, trailing v44 wired last),
  `GatchaDrop` (`@0x7509c0`), `HeroEntry` (`@0x76f47c`),
  `LogicPlayer` (`@0x6bdfb0/@0x89bacc`, incl. early +84 and +56-before-+24),
  `LogicHeroUpgrades` (`@0x5d26c8`), `LogicBattleEmotes` (`@0x759184`).
- `LogicOfferBundle` (`@0x69b644`) + leaves `LogicGemOffer` (`@0x93ed54`),
  `ChronosTextEntry` (`@0x7b6d10`; vtable slot +0x20 resolved via the
  ByteStream vtable to `writeStringReference @0x608f14`),
  `AdStatus` (`@0x8d21fc/@0x566588`), `ProLeagueSeasonData` (`@0x71b258`),
  `BrawlPassSeasonData` (`@0x5e8140`), `LogicQuests` (`@0x1ae36c`),
  `VanityItems` (`@0x5980fc`), `LogicBitList` (`@0x9740d8/@0x5c241c`, C2 `@0x7c7788`),
  `LogicPlayerRankedSeasonData` (`@0x286950/@0x97acd8`) + `LogicPlayerRewardData`
  (`@0x243790`), `VanityItemEntry` (`@0x427c6c`) + `VanityItemProp` (`@0x8bd8d0`),
  `LogicRewardConfig` (`@0x88d288/@0x710eb4`) + `LogicCondition` (`@0x89d570`).

## Verify flags (against live captures later)

- Duplicate numeric ids: 20101 (CreateAccount* vs UdpBigFragment),
  24116 (DebugNewbie vs HomeBattleReplayFailed), 24123 (DailyEvents vs
  SeasonRewards), 14103 (MatchmakeRequest vs StartGame),
  14104 (StartMission vs StartSpectate), 25892 (Disconnected vs Titan*).
- `TeamInviteStatusMessage` 24582 encode/decode flag asymmetry.
- `LoginOkMessage` f329not inversion.
- int-list wire type (`TeamSetLocation`, `TeamSetPlayerMap`): vint assumed.

## Next (priority order)

1. Pending nested entries wave 2: replace `messages/pending/*` stubs with
   real wire formats, one class per file (see Layout rule below).
2. `LogicDataTables` follow-ups: `datatable_map.csv` coverage for the
   remaining `csv_client` tables.
3. `Logic*` gameplay core by subsystem (`LogicCharacter*`,
   `LogicBattleMode*`, …).
4. `sc` engine (`String`, `LogicStringUtil`, `ResourceManager`, …).
5. Platform bridges (`titan/*`, JNI) as clean interfaces.

Every new function cites its IDA address and lands with a test where the
wire format or logic is observable. Progress is enforced by
`tools/generate_registry.py`: the registry lists all 37,064 functions and
their status, and it is compiled into the binary.

## Layout rule (strict): one class per file

- `include/titan/messages/<Class>.hpp` (+ `src/messages/<Class>.cpp`
  when it has out-of-line bodies) — 335 messages.
- `include/titan/commands/<Class>.hpp` — 59 commands.
- `include/titan/messages/pending/<Name>.hpp` — one pending nested entry
  per file (replaced in place by the real class when reversed).
- Shared bases live in `messages/Common.hpp`; aggregates
  (`AllMessages.hpp`, `AllCommands.hpp`) are generated.
- New code must follow this: no two message/command classes share a file.
