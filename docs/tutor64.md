
# Reverse engineering (ARM64)

адаптация [tutor.md](./tutor.md) под 64 бита. все адреса ниже — живые, из нашего IDB `libg_decrypted.so` (v36, arm64). если у тебя другой дамп — адреса поедут, методика нет.

## что поменялось с 32 бит

коротко, чтобы не ловить сюрпризы:

- указатели по **8 байт** вместо 4 → все смещения в vtable **умножились на 2**. `writeVInt` был `+64`, стал `+128` (`0x80`). это тот же самый слот (16), просто `16 * 8 = 128`.
- в декомпиляторе Hex-Rays смещения показываются в **десятичном** виде: `*(_QWORD *)a2 + 128LL` — это `0x80`, writeVInt. не путайся.
- поля объектов разъехались: ByteStream внутри сообщения лежит по `a1 + 16` (был `a1 + 8`), поля типа `accountPreset` — по `a1 + 132` (был `a1 + 80`).
- имедиаты в ARM64 грузятся иначе: вместо `MOVW/MOVT` — `MOV W0, #imm` (если влезает в 16 бит) или пачка `MOVZ/MOVK`. все ID сообщений < 65536, так что это всегда одна инструкция.

## начинаем реверс инжинеринг

нужен дизассемблер. самый распространенный в SCRE это IDA Pro.

> где его скачать - зависит уже от тебя.

библиотека называется libg.so. открыть сырую либу можно, но строки зашифрованы, а почти весь реверс на них и держится.

## получаем строки

дампай либу из памяти (PADumper и т.п.), не забудь Fix ELF.

> запусти Brawl Stars, открой PADumper, выбери Brawl Stars, напиши libg.so в поле библиотеки, поставь галочку Fix ELF.

## анализируем

в v36 есть дебаг символы (DWARF Info). в 64-битном дампе имена **уже нормальные** — мангленые, но валидные:

```
_ZN10ByteStream9writeVIntEi
_ZNK19ResetAccountMessage14getMessageTypeEv
_ZN14LogicDailyData6encodeEP15ChecksumEncoder
```

IDA их сама деманглит в `ByteStream::writeVInt(int)` и т.д. скрипт-фикс из tutor.md (который лепил `_` перед именем) тут **не нужен** — он был для битых имён в arm32 дампе. если вдруг имена битые у тебя — сначала проверь, что деманглер включен (Options → Demangled names → Show demangled).

если символов нет вообще — не беда, ниже есть раздел как жить без них.

## находим структуру сообщений

возьмем для примера сообщение 10116, ResetAccountMessage.

в IDA: Search → Search immediate value, галочка Find all occurrences, в поле `10116` (или `0x2784`).

в arm32 это был `MOVW R0, #0x2784`. в arm64 ищи такое:

```asm
_ZNK19ResetAccountMessage14getMessageTypeEv:
MOV W0, #0x2784      ; 0x6cdf20
RET
```

две инструкции — загрузил константу и вернулся. это метод `::getMessageType`.

> ложных срабатываний будет море (любое число 10116 в коде/данных). фильтр простой: ищи именно `MOV W0, #...` + `RET` — это геттер типа сообщения.

### vtable сообщения

идем по xref на `getMessageType` (в нашем IDB это `0x6cdf20`). попадаем в vtable:

```
0xd5b270   DCD ResetAccountMessage::~ResetAccountMessage   ; D2 (vptr указывает сюда)
0xd5b278   DCD ResetAccountMessage::~ResetAccountMessage   ; D0
0xd5b280   DCD ResetAccountMessage::encode                 ; ::encode
0xd5b288   DCD ResetAccountMessage::decode                 ; ::decode
0xd5b290   DCD ResetAccountMessage::getServiceNodeType
0xd5b298   DCD ResetAccountMessage::getMessageType         ; <-- отсюда пришли
0xd5b2a0   DCD PiranhaMessage::getMessageTypeName
0xd5b2a8   DCD ResetAccountMessage::destruct
```

раскладка **один в один** как в 32-битном туторе, просто записи по 8 байт:

| слот | смещение | метод |
|------|----------|-------|
| 0 | 0x00 | ~Message (D2) |
| 1 | 0x08 | ~Message (D0) |
| 2 | 0x10 | encode |
| 3 | 0x18 | decode |
| 4 | 0x20 | getServiceNodeType |
| 5 | 0x28 | getMessageType |
| 6 | 0x30 | getMessageTypeName |
| 7 | 0x38 | destruct |

перед vptr (vptr − 16) лежит offset-to-top (=0), перед ним (vptr − 8) — typeinfo. по ним удобно проверять, что это точно начало vtable.

нам нужен `::encode` (пишет методами записи) или `::decode` (методами чтения). лучше `encode`.

### открываем encode

```c
__int64 __fastcall ResetAccountMessage::encode(__int64 a1, int a2)
{
  PiranhaMessage::encode(a1, a2);
  return ByteStream::writeInt((ChecksumEncoder *)(a1 + 16), *(_DWORD *)(a1 + 132));
}
```

- `a1` — this (само сообщение)
- `a1 + 16` — встроенный ByteStream (в arm32 был `a1 + 8`)
- `*(_DWORD *)(a1 + 132)` — поле сообщения (в arm32 было `a1 + 80`)

структура этого сообщения — один `writeInt`. всё, дальше по аналогии.

## непонятные смещения в структуре

на больших сообщениях (OwnHomeData и компания) вместо прямых вызовов поедет вот такое:

```c
(*(void (__fastcall **)(ChecksumEncoder *, _QWORD))(*(_QWORD *)a2 + 128LL))(a2, *a1);
(*(void (__fastcall **)(ChecksumEncoder *, _QWORD))(*(_QWORD *)a2 + 128LL))(a2, a1[1]);
(*(void (__fastcall **)(ChecksumEncoder *, _QWORD))(*(_QWORD *)a2 + 128LL))(a2, a1[2]);
```

`*(_QWORD *)a2 + 128LL` — смещение функции в vtable, `a1[1]` (`a1[2]`, ...) — поле объекта, откуда берутся данные. это живой кусок из `LogicDailyData::encode` (`0x6985dc`), первые строки — серия writeVInt.

таблица смещений vtable **ByteStream** для arm64 (vptr → `0xd5c468` в нашем IDB, снято целиком):

| смещение (hex) | смещение (dec, как в Hex-Rays) | функция |
|----------------|-------------------------------|---------|
| 0x38 | 56  | writeString() |
| 0x40 | 64  | writeBoolean() |
| 0x48 | 72  | writeInt() |
| 0x50 | 80  | writeInt8() |
| 0x58 | 88  | writeInt16() |
| 0x60 | 96  | writeInt24() |
| 0x68 | 104 | writeBytes() |
| 0x70 | 112 | writeByte() |
| 0x78 | 120 | writeShort() |
| 0x80 | 128 | writeVInt() |
| 0x88 | 136 | writeVLong() |
| 0x90 | 144 | writeLongLong(long) |
| 0x98 | 152 | writeLong(LogicLong) |
| 0xA8 | 168 | readStringReference() |
| 0xB0 | 176 | readString() |
| 0xD0 | 208 | readBoolean() |
| 0xD8 | 216 | readInt() |
| 0xF8 | 248 | readVInt() |

полная vtable (для сверки слотов):

```
+0x00 ~ByteStream (D2)          +0x70 writeByte
+0x08 ~ByteStream (D0)          +0x78 writeShort
+0x10 destruct                  +0x80 writeVInt
+0x18 isCheckSumOnlyMode        +0x88 writeVLong
+0x20 writeStringReference      +0x90 writeLongLong
+0x28 writeFilteredStringRef    +0x98 writeLong(LogicLong)
+0x30 writeFilteredString       +0xA0 isByteStream
+0x38 writeString               +0xA8 readStringReference
+0x40 writeBoolean              +0xB0 readString
+0x48 writeInt                  +0xD0 readBoolean
+0x50 writeInt8                 +0xD8 readInt
+0x58 writeInt16                +0xE0 readInt8
+0x60 writeInt24                +0xE8 readInt16
+0x68 writeBytes                +0xF8 readVInt
```

сравнение с 32-битной таблицей из tutor.md — слоты те же, байты x2:

| функция | 32 бита (tutor.md) | слот | 64 бита |
|---------|-------------------|------|---------|
| writeString | 28 | 7 | 0x38 (56) |
| writeBoolean | 32 | 8 | 0x40 (64) |
| writeInt | 36 | 9 | 0x48 (72) |
| writeVInt | 64 | 16 | 0x80 (128) |
| writeLongLong | 72 | 18 | 0x90 (144) |

> ⚠️ в tutor.md `writeLogicLong = 88` (слот 22), но в нашем arm64 дампе `writeLong(LogicLong)` лежит в слоте 19 (`0x98`), а слот 22 — уже `readString`. таблица из тутора снята с arm32 v36.218 и местами не бьется. **таблица выше снята с нашего IDB — она источник истины**, при расхождениях верь ей.

## если символов нет

vtable безымянные, но ищутся за секунду через data-xref. любой известный метод (например `ByteStream::writeVInt`) в vtable упоминается ровно как указатель:

1. находишь тело `writeVInt` (по характерному зигзагу: сравнения с `0x40`/`0x80`, сдвиги на 6/13/20/27 — он один в один как `ByteStream.WriteVInt` в нашем сервере `src/LaserTitan/DataStream/ByteStream.cs`).
2. xrefs to → тип Data → получаешь адрес слота vtable (у нас это `0xd5c4e8`).
3. листаешь назад до пары `~D2/~D0` — это начало vtable. дальше по таблице выше.

для сообщений без имён — search immediate по ID, дальше как обычно.

## шпаргалка по IDA MCP

работаем через MCP-сервер, основные инструменты:

- `find` (type=immediate) — поиск ID сообщения по всему образу
- `xrefs_to` / `xref_query` (xref_type=data) — из геттера в vtable
- `decompile` — псевдокод encode/decode
- `func_query` (filter=`*Message*encode*`) — когда имена есть, всё ищется по имени напрямую

пример для ResetAccountMessage в один заход: `func_query *ResetAccountMessage*` → видим encode (`0x4465cc`), decode (`0x93d3c8`), getMessageType (`0x6cdf20`) → `decompile` encode → структура готова.

---

готово. методика из tutor.md для arm64 рабочая, все смещения пересчитаны и проверены на живом IDB.
