
# Reverse engineering

25 декабря 2024 в 3:42

## Начинаем реверс инжинеринг.

Чтобы начать реверс инжинеринг, тебе нужен дизассемблер.

Самый распространенный в SCRE это IDA Pro.

> Где его скачать - зависит уже от тебя.

## че дальше

*Дальше, я думаю, что ты уже знаком хотя бы с каким либо языком программирования и как они работают. я не профи и вообще сам почти не ебу чо как делат.*

Давайте начнем. Библиотека называется libg.so.

Ты мажешь открыть libg.so через IDA Pro, но это будет бессмысленно, так как все строки зашифрованы. А почти весь реверс инжинеринг на нем и держится!

## Получаем строки.

Получить строки довольно легко.

Тебе просто надо дампнуть библиотеку, можешь использовать PADumper. Он (наверное) самый простой.

Но он требует рут, это так, к слову.

> Запусти Brawl Stars, открой PADumper, выбери Brawl Stars, и напиши libg.so в поле библиотеки. Не забудь поставить галочку на Fix ELF, так как оно (я думаю) нормально так фиксит либу.

Ну вот и все. Теперь можно анализировать либу и находить "оффсеты".

## Анализируем

Ну вообще, анализировать сырую либу не так бесполезно. Вот, к примеру Brawl Stars v36.218. В нем есть дебаг символы (DWARF Info).

Но что же дают эти "дебаг символы"? Названия функций, самая важная штука, которая позволяет ориентироваться в либе. Но названия там немного сломаны. Их можно починить при помощи скрипта от Primo:

```c
//      This script fixes v36 bs lib's glitched debug symbols.
//

#include <idc.idc>

static main() {
    auto ea,x;

    for ( ea=get_next_func(0); ea != BADADDR; ea=get_next_func(ea) ) {
        x = get_func_flags(ea);

        auto func_name = GetFunctionName(ea);
        auto new_name = "_" + func_name;
        MakeName(ea, new_name);


    }
    msg("All functions got patched with success!");
}
```

и названия функций

```
_ZNK12GameSettings12is5v5EnabledEv
_ZN17EventDetailsPopup17setAutoCloseStateEb
_ZN17AllianceTeamEntry12idListMatchEP14LogicArra...
_ZN13ListContainerC3EP7MovieClipRKSStringS4_S4_
_ZN20LogicCharacterServer32getCarryableLastOwne...
u_ispace_S8
_ZN25KickAllianceMemberMessageC2Ev
_ZN23PublishPlayerMapResponseMessage12setErrorC...
_ZN4Rect12movePositionEff
_ZN8SelfhelpD2Ev
_ZN21GlobalChatLineMessage11setAvatarIdEP9LogicId
_ZN4icu_5812DictionaryBreakEngine7handlesEi
_ZNK20LogicCharacterServer16getMovementSpeedEi
_ZN22AllianceChatController19takeStreamOwnerShipC...
_ZN22HeroPowerRewardNotification8destructEv
_ZN25SetHeroDefaultAnimationD2Ev
_ZN11Character3D18getActiveAnimationEi
_ZN27LogicUnlockFreeSkinsCommand8destructEv
_ZN21LogicProjectileServer13runEarlyTicksEv
_ZNK13LogicTileData21getMapEditorConnectedEv
_ZN23RankedRewardsTrackPopupD2Ev
_ZN13ShopTagElementC2EP4ShopRKSStringS4_P20Shc...
_ZNK31PurchaseOfferBundlePopupHandler35sendAnc...
_ZN19SelectLocationPopup6addBTHREF16DropGUICont...
_ZN2EFriendOnlineStatusMessage6decodeEv
_ZThn72_N17AnimationInstanceD0Ev
_ZN19ResourceManager21getAbsoluteUpdatePathER...
_ZN17LogicHeroUpgradeScene6codeEP15ChecksumEncc...
_ZN22TeamGearInviteMessage8destructEv
_ZNK13DisplayObject20getBounds_deprecatedEPK6S...
```

превратятся в

```
GameSettings::is5v5Enabled(void)
EventDetailsPopup::setAutoCloseState(bool)
AllianceTeamEntry::idListMatch(LogicArrayList<Log...
ListContainer::ListContainer(MovieClip *,int,int,St...
LogicCharacterServer::getCarryableLastOwnerPlaye...
u_ispace_S8
KickAllianceMemberMessage::KickAllianceMemberMes...
PublishPlayerMapResponseMessage::setErrorCode(i...
Rect::movePosition(float, float)
Selfhelp::~Selfhelp()
GlobalChatLineMessage::setAvatarId(LogicLong *)
icu_58::DictionaryBreakEngine::handles(int,int)
LogicCharacterServer::getMovementSpeed(int,int)
AllianceChatController::takeStreamOwnerShip(void)
HeroPowerRewardNotification::destruct(void)
SetHeroDefaultAnimation::~SetHeroDefaultAnimatio...
Character3D::getActiveAnimation(int)
LogicUnlockFreeSkinsCommand::destruct(void)
LogicProjectileServer::runEarlyTicks(void)
LogicTileData::getMapEditorConnected(void)
RankedRewardsTrackPopup::~RankedRewardsTrack...
ShopTagElement::ShopTagElement(Shop *,String const...
PurchaseOfferBundlePopupHandler::sendAnalyticEve...
SelectLocationPopup::addBTHREF(DropGUIContainer *...
```

К сожалению, эта версия все еще имеет хх шифрование строк. Но я оставлю ссылочку на дамп этой библиотеки. (arm32). https://mega.nz/file/e3AB3YqQ#Z1y4M-9rIUvA274iZgvKuqeq2k17ZjBEhu5SxqZMQXY

## Находим структуру сообщений

Чтобы найти структуру сообщений, надо сначала найти ее VFTable (Virtual Function table) через IDA Pro.

Давайте найдем... например.. сообщение 10116, ResetAccountMessage. в IDA Pro, в разделе Search, нажми Search immediate value, также должна стоять галочка рядом с Find all occurrences и в поле ввода введи айди сообщения.

```
Address                       Function      Instruction
.text&ARM.extab:0052C760      sub_52C760    MOVW    R0, #0x2784
.ARM.exidx:0084FB4                          DCB 0x84
.ARM.exidx:008590B0                         DCB 0x84
.data:0091C8F2                              DCB 0x84
```

Вон оно, самое первое!

```c
int sub_52C768()
{
  return 10116;
}
```

Итак, мы в методе ::getMessageType method. Давайте перейдем к xref.

```
LOAD:008DA2E4     DCD sub_7ACD5C
LOAD:008DA2E8     DCD sub_754E78
LOAD:008DA2EC     DCD sub_90264
LOAD:008DA2F0     DCD sub_D206C
LOAD:008DA2F4     DCD sub_712BAC
LOAD:008DA2F8     DCD sub_52C768
LOAD:008DA2FC     DCD sub_6774D8
LOAD:008DA300     DCD sub_218B54
```

Мы видим функции (методы) в памяти. Они расположены также, как и в других сообщениях, потому давайте добавим немного комментариев:

```
LOAD:008DA2E4           DCD sub_7ACD5C        ; ::~PiranhaMessage
LOAD:008DA2E8           DCD sub_754E78        ; ::~ResetAccountMessage
LOAD:008DA2EC           DCD sub_90264         ; ::encode
LOAD:008DA2F0           DCD sub_D206C         ; ::decode
LOAD:008DA2F4           DCD sub_712BAC        ; ::getServiceNodeType
LOAD:008DA2F8           DCD sub_52C768        ; ::getMessageType
LOAD:008DA2FC           DCD sub_6774D8        ;
PiranhaMessage::getMessageTypeName
LOAD:008DA300           DCD sub_218B54        ; ::destruct
```

Нам нужен именно ::encode или ::decode, но лучше всего подойдет ::encode. Реальная разница в том, что ::encode использует методы записи, а ::decode использует методы чтения.

Давайте откроем ::encode.

```c
int __fastcall sub_90264(int a1)
{
  nullsub_50();                            // это PiranhaMessage::encode
  return sub_25E70C(a1 + 8, *(_DWORD *)(a1 + 80));
}
```

И мы видим, что для записи используется один метод - sub_25E70C. Давайте представим, что путем крутых реверс штук, мы выяснили, что это ByteStream::writeInt (и не посмотрели на скрипты других людей!!!). Ну и вот, структура этого сообщения - записывание числа.

## Непонятные смещения в структуре

Представим, что вы, такие крутые, решили перепроверить структуру OwnHomeDataMessage, но вот незадача: вместо функций вызывается смещение объекта! Например:

```c
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData);
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData + 4));
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData + 8));
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData + 12));
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData + 16));
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData + 20));
(*(*ChecksumEncoder + 64))(ChecksumEncoder, *(LogicDailyData + 24));
```

Вот ChecksumEncoder + 64 - это само смещение на функцию, LogicDailyData + 4(+ 8, + 12 и так далее) - это смещение ByteStream, откуда начинать записывать данные.

Чтобы далеко не бегать, вот вам смещения и что они значат:

| Смещение | Функция |
|----------|------------------|
| 28       | writeString()    |
| 32       | writeBoolean()   |
| 36       | writeInt()       |
| 64       | writeVInt()      |
| 72       | writeLong()      |
| 88       | writeLogicLong() |

---

Готово — это вся статья целиком в текстовом виде. Если нужно, могу дополнительно добавить в начало короткое пояснение-контекст (что это гайд по реверс-инжинирингу клиента Brawl Stars, библиотека `libg.so`, инструменты IDA Pro + PADumper) — иногда другой ИИ быстрее «въезжает», когда ей сразу дают такую преамбулу. Скажи, и я добавлю.