# TitanBrawl36 wire protocol (Brawl Stars v36)

All facts verified against `libg_decrypted.so` via IDA (see
`docs/IDA_BASELINE.md`). This is the contract any client (and later, GUI)
speaks.

## Transport framing

Every message goes on the wire as header + payload
(`Messaging::writeHeader @0x46664c`, `readHeader @0x93da60`,
`encryptAndWrite @0x93221c`):

```
[u16be type][u24be payloadLen][u16be version][payload...]   (7 bytes)
```

- `payloadLen` covers the payload only and must be `< 0x1000000`,
  else the binary calls `Debugger::error`.
- The version is the message's `getMessageVersion` (`this+8`, `@0x571a94`).
- Same layout on TCP and HTTP (`HTTPMessaging::writeMessage @0x562efc`).
- In code: `titan::net::writeHeader/readHeader/encodeFrame/encodeSendFrame`.

## Session crypto

- `ClientHello` (10100) / `Login` (10101) go out as plaintext memcpy.
- Everything else goes through the session `PepperEncrypter`
  (NaCl secretbox, vendored tweetnacl): ciphertext = plaintext + 16B MAC,
  header computed over the FINAL length (`@0x93221c`).
- Nonce starts random (`@0x46a64c`), then LE `+= 2` per message
  (`nextNonce @0x6fb2ac`), before each encrypt/decrypt.
- Key exchange: NaCl box + Blake2b (`PepperCrypto::box_open @0x5863c0`,
  `box @0x3677b0`; port: `titan::crypto::pepperBoxSeal/pepperBoxOpen`,
  padded tweetnacl API, inverted-failure convention mapped to nullopt);
  `LoginFailed`-family response (20103) carries the 24B server nonce +
  32B session key; decrypt failures answer with `CryptoError`.
- OPEN QUESTION (next wave): live traffic may use
  `PepperPerMessageEncrypter::encrypt @0x39d734` (per-message nonce prefix
  on the wire + secretbox) instead of the rolling-nonce `PepperEncrypter`;
  the session-setup call site decides. Our `SodiumPepper` backend models
  the simple form; do NOT attempt real logins until this is resolved.
- Port: libsodium behind `TITAN_WITH_SODIUM`; `titan::crypto::Encrypter`.

## Message dispatch

- `LogicLaserMessageFactory::createMessageByType @0x513e58`: numeric type
  → message instance (`titan::createMessageByType`, generated).
- Known duplicate ids (one factory winner each, rest verified per getter —
  full list in `data/factory_notes.csv`): 20101, 24116, 24123, 14103,
  14104, 25892, 20000, 10109, 19002, 23456, 24109.
- UDP-mode flag (`*(factory+8)`): only the UDP subset
  (10109/19002/23456/24109/29002); our default choices match it.

## Payload encoding (`ByteStream`, embedded at message `this+16`)

- `writeInt`: 4 bytes big-endian. `writeVInt`: 1..5 byte sign-aware varint
  (6-bit head + `0x40` sign). `writeVLong`: 1..10 bytes, same scheme in 64b.
- `writeBoolean`: bit-packed into the current byte.
- `writeString`: `writeInt(byteLength)` + bytes, null → `-1`, `>= 900001`
  rejected. `writeStringReference`: same, never null.
- `writeBytes`: `writeInt(len)` + bytes, null → `-1`.
- `DataReference`: null → single `VInt 0`, else classId + instanceId vints.
- `LogicLong`: two ints (high, low); vint-pair form via `encodeLogicLong`.
- Every write folds the checksum (`ChecksumEncoder`, state at `this+12`).
- Nested game classes (`LogicDailyData`, `LogicClientHome`, …):
  count-vint + elements; optional object = bool + body;
  nullable list = count or `-1` for null (`writeIntList`).

## Login flow (message ids)

`ClientHello` 10100 → `Login` 10101 (or `LoginUsingSession` 10102) →
server answers `LoginOk`/`LoginFailed` → `OwnHomeData` 24101
(home + avatar: `LogicClientHome` + `LogicClientAvatar`) →
`EndClientTurn` 14102 / game traffic. Keep-alive pair 10108/20108.

## Analytics + home notifications

- `AnalyticEventMessage` 10110: stringref name + JSON params string
  (compact, `{"k":"v"}`; parser/serializer in `game/LogicJSON*`).
- Home notifications: `vint type` + body, built by
  `NotificationFactory::create @0x5f5d50` (char-range ids 63–94).

## Regression cover

- `test_factory`: factory + header layout + send-path incl. 10100/10101 bypass.
- `test_session`: full loopback (frame → parse → factory → decode),
  plaintext + encrypted legs, version, rejection paths.
- `test_entries` / `test_msgbatch`: every nested entry + message batch.
- `test_json`: JSON subsystem + `AnalyticEvent`.
