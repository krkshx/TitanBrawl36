# Reverse status — every function accounted for

Inventory: **37,064** functions (`data/ida_shard_00..14.csv`), all with
unique addresses, see `docs/IDA_BASELINE.md`.
Registry (compiled in): **1646 reimplemented, 9718 third-party, 25700 pending**.

| Bucket | Count | Handling |
|---|---|---|
| Third-party (`ThirdPartyExternal`) | 9718 | system libs, see `docs/THIRDPARTY.md` |
| Game code reimplemented | 1646 fns / 329 classes | `src/` + tests |
| Game code pending | 25700 | `FunctionRegistry` status `Pending` |

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

## Verify flags (against live captures later)

- Duplicate numeric ids: 20101 (CreateAccount* vs UdpBigFragment),
  24116 (DebugNewbie vs HomeBattleReplayFailed), 24123 (DailyEvents vs
  SeasonRewards), 14103 (MatchmakeRequest vs StartGame),
  14104 (StartMission vs StartSpectate), 25892 (Disconnected vs Titan*).
- `TeamInviteStatusMessage` 24582 encode/decode flag asymmetry.
- `LoginOkMessage` f329not inversion.
- int-list wire type (`TeamSetLocation`, `TeamSetPlayerMap`): vint assumed.

## Next (priority order)

1. Message factory dispatch (`LogicLaserMessageFactory`) + transport
   framing (header/length prefix, send path).
2. Nested entries wave 1: `LogicClientAvatar`, `AllianceHeaderEntry`,
   `AllianceFullEntry`, `AllianceMemberEntry`, `LogicCommand`,
   `LogicCompressedString`, `ByteStreamHelper::decompress`.
3. `LogicDataTables` + CSV (`assets/csv_logic`, `assets/csv_client`).
4. `Logic*` gameplay core by subsystem (`LogicCharacter*`,
   `LogicBattleMode*`, …).
5. `sc` engine (`String`, `LogicStringUtil`, `ResourceManager`, …).
6. Platform bridges (`titan/*`, JNI) as clean interfaces.

Every new function cites its IDA address and lands with a test where the
wire format or logic is observable. Progress is enforced by
`tools/generate_registry.py`: the registry lists all 37,064 functions and
their status, and it is compiled into the binary.
