#pragma once
#include "LogicConfData.cpp"
#include "../player/LogicDailyData.cpp"
#include "../data/GatchaDrop.cpp"
#include "../data/LogicDataSlot.cpp"
#include <string>
#include <vector>

class LogicClientHome {
public:
    LogicDailyData daily;
    LogicConfData conf;
    std::int64_t long16 = 0;
    std::vector<BaseNotification> notifications;
    bool notifTruncated = false;
    bool b72 = false;
    std::vector<GatchaDrop> drops;
    std::vector<std::int32_t> refs;
    bool ok = false;

    void decode(ByteStream &s) {
        daily.decode(s);
        conf.decode(s);
        // По либе (_ZN15LogicClientHome6decodeEP10ByteStream @ 0x3b1044):
        // long (+272 = readLong, 8 байт), VInt count, фабрика уведомлений,
        // затем bool и только потом массив GatchaDrop (у нас было наоборот).
        long16 = s.readLongLong();
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                std::int32_t t = s.readVInt();
                if (!NotificationFactory::isKnownType(t) || notifTruncated) {
                    notifTruncated = true;
                    break;
                }
                notifications.push_back(BaseNotification::decode(s, t));
            }
            if (notifTruncated) {
                return;
            }
        }
        b72 = s.readBoolean();
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                drops.push_back(GatchaDrop::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                refs.push_back(ByteStreamHelper::readDataReference(&s));
            }
        }
        ok = true;
    }
};
