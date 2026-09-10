# Reverse status — every function accounted for

Inventory: **37,064** functions (`data/ida_shard_00..14.csv`), all with
unique addresses, see `docs/IDA_BASELINE.md`.
Registry (compiled in): **2096 reimplemented, 9717 third-party, 25251 pending**
(`titan_registry_test` is the source of truth — update these numbers when it moves).

| Bucket | Count | Handling |
|---|---|---|
| Third-party (`ThirdPartyExternal`) | 9717 | system libs, see `docs/THIRDPARTY.md` |
| Game code reimplemented | 2096 fns | `src/` + tests |
| Game code pending | 25251 | `FunctionRegistry` status `Pending` |

## Done

- `ChecksumEncoder` — writeBoolean/writeVInt/writeInt/writeStringLength/
  writeStringReference (`0x534690 0x5c50e0 0x8a558c 0x6ed97c 0x69a564`)
- `ByteStream` — write path (`0x3294c0 0x3f3bd0 0x87bc64 0x298f64
  0x5174d0 0x61bd08 0x608f14`) + read path (`0x23bc10 0x191dc4 0x356c40
  0x28f62c`) + byte/short/longlong + `isAtEnd` (`0x7625a8`) + raw-bytes
- `PiranhaMessage` — encode guard no-op (`0x8f2e70`)
- `VLong` wire type: `ChecksumEncoder::writeVLong` (`0x739a8c`, chained
  64-bit fold), `ByteStream::writeVLong` (`0x3ec094`, 1..10 byte sign-aware
  varint: 6-bit head + 0x40 sign + LEB128 groups), `ByteStream::readVLong`
  (`0x8d3af0`, per-length sign extension); `LogicUuid` (`0x29dc9c`)
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
  `LogicHeroUpgrades` (`@0x5d26c8`), `LogicBattleEmotes` (`@0x759184`),
  `HeroDataEntry` (`@0x72d8ec`), `DeliveryUnit` (`@0x4481e8`),
  `LogicMilestoneProgress` (`@0x6b920c`), `AllianceWarFaction` (`@0x246da0`),
  `AllianceWarNode` (`@0x5f7f9c`), `LogicVector2` (`@0x29fda4`, plain ints),
  `XpEntry` (`@0x67ddd4`). `AllianceTeamEntry` re-verified against `@0x538d6c`
  (incl. `writeLong @0x3f4e60` = `LogicLong::encode`); no change needed.
- `LogicOfferBundle` (`@0x69b644`) + leaves `LogicGemOffer` (`@0x93ed54`),
  `ChronosTextEntry` (`@0x7b6d10`; vtable slot +0x20 resolved via the
  ByteStream vtable to `writeStringReference @0x608f14`),
  `AdStatus` (`@0x8d21fc/@0x566588`), `ProLeagueSeasonData` (`@0x71b258`),
  `BrawlPassSeasonData` (`@0x5e8140`), `LogicQuests` (`@0x1ae36c`),
  `VanityItems` (`@0x5980fc`), `LogicBitList` (`@0x9740d8/@0x5c241c`, C2 `@0x7c7788`),
  `LogicPlayerRankedSeasonData` (`@0x286950/@0x97acd8`) + `LogicPlayerRewardData`
  (`@0x243790`), `VanityItemEntry` (`@0x427c6c`) + `VanityItemProp` (`@0x8bd8d0`),
  `LogicRewardConfig` (`@0x88d288/@0x710eb4`) + `LogicCondition` (`@0x89d570`).
- Notification wave (COMPLETE): `BaseNotification` (`@0x7beae0/@0x605478`)
  + all 29 subclasses, each with verified type id (getters `@0x9aca40`…),
  factory `NotificationFactory::create @0x5f5d50` fully mapped (63–94;
  2/Donate intentionally unmapped — binary has no case either; 83/87
  vtables unidentified); `LogicClientHome` notifications decode for real.
  Shapes: base-only (FloaterText 66, Qualify 76), vint×1/2/3/5, string
  (Donate), display (Band 82), + specials (RankedMidSeason 67,
  RankedSeasonEnd 68, BrawlPassAutoCollect 69, StarPoints 79 + ScoreEntry,
  Revoke 85, SkinPurchaseOption 65, ChallengeReward 63/70 + LogicRewards stub).
  `RankingEntry` (`@0x770f68/@0x88e8d8`; slots resolved to
  `PlayerRankingData` (`@0x8a6d9c`) + `AllianceRankingData` (`@0x6ef580`))
  + `decodeLogicLong` helper.
  JSON subsystem: node model (Null/Boolean/Number/String/Array/Object),
  recursive-descent parser (BOM skip, ws<=32, escapes incl. \uXXXX, int
  exact / float-through-float32, empty forms, fail->null) and compact
  serializer — verified against parseValue @0x8909a4, parseObject @0x90fb70,
  parseArray @0x6ef9d0, parseNumber @0x7c35ec, writeString @0x614418,
  writeToString ×6, createJSONString @0x595724.
  `AnalyticEvent` (`@0x2787dc/@0x945888`) live via JSON (own test_json).

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

## Layout rule (strict): one class per file, one home per class

- `include/titan/messages/<Class>.hpp` (+ `src/messages/<Class>.cpp`
  when it has out-of-line bodies) — 335 messages.
- `include/titan/commands/<Class>.hpp` — 59 commands.
- `include/titan/game/<Class>.hpp` (+ `src/game/<Class>.cpp` when big) —
  every reversed nested/data class lives here, no exceptions.
- `include/titan/messages/pending/` — ONLY not-yet-reversed stubs
  (`TITAN_PENDING_ENTRY`) plus documented `using` aliases where the stub
  name differs from the binary's class. No forwarders: when a stub gets
  reversed, its pending file is DELETED and includers switch to `game/`.
- Shared bases live in `messages/Common.hpp`; aggregates
  (`AllMessages.hpp`, `AllCommands.hpp`) are generated.
- New code must follow this: no two message/command classes share a file.
