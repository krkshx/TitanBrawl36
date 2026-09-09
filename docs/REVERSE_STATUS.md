# Reverse status — every function accounted for

Inventory: **37,064** functions (`data/ida_shard_00..14.csv`), all with
unique addresses, see `docs/IDA_BASELINE.md`.

| Bucket | Count | Handling |
|---|---|---|
| Third-party (`ThirdPartyExternal`) | ~9,926 | system libs, see `docs/THIRDPARTY.md` |
| Game code reimplemented | growing | `src/` + tests |
| Game code pending | rest | `FunctionRegistry` status `Pending` |

## Done

- `ChecksumEncoder` — writeBoolean/writeVInt/writeInt/writeStringLength
  (`0x534690 0x5c50e0 0x8a558c 0x6ed97c`)
- `ByteStream` — write path (`0x3294c0 0x3f3bd0 0x87bc64 0x298f64
  0x5174d0 0x61bd08`) + read path (`0x23bc10 0x191dc4 0x356c40`)
- `PiranhaMessage` — encode guard no-op (`0x8f2e70`)
- `ResetAccountMessage` 10116 (`0x6cdf20 0x4465cc 0x93d3c8 0x9a5238`)
- `KeepAliveMessage` 10108 / `KeepAliveServerMessage` 20108
  (`0x663228 0x7db92c 0x265844 0x977fa8 0x91db1c 0x2e0090`)

## Next (priority order)

1. All remaining `*Message::encode/decode/getMessageType` (protocol first).
2. `ByteStream` remainder: writeByte/writeShort/writeLong/writeVLong/
   read side for them, `ByteStreamHelper::decompress`.
3. `LogicDataTables` + CSV (`assets/csv_logic`, `assets/csv_client`).
4. `Logic*` gameplay core by subsystem (`LogicCharacter*`,
   `LogicBattleMode*`, `LogicCommand*`, …).
5. `sc` engine (`String`, `LogicStringUtil`, `ResourceManager`, …).
6. Platform bridges (`titan/*`, JNI) as clean interfaces.

Every new function cites its IDA address and lands with a test where the
wire format or logic is observable. Progress is enforced by
`tools/generate_registry.py`: the registry lists all 37,064 functions and
their status, and it is compiled into the binary.
