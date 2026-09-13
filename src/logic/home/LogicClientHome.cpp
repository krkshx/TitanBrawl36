#pragma once
#include "LogicConfData.cpp"
#include "../player/LogicDailyData.cpp"
#include "../../data/GatchaDrop.cpp"
#include "../../data/LogicDataSlot.cpp"
#include <string>
#include <vector>

class LogicClientHome {
public:
    LogicDailyData daily;
    LogicConfData conf;
    std::string str16;
    std::vector<BaseNotification> notifications;
    bool notifTruncated = false;
    std::int32_t i76 = 0;
    bool b72 = false;
    std::vector<GatchaDrop> drops;
    std::vector<std::int32_t> refs;
    bool ok = false;

    void decode(ByteStream &s) {
        daily.decode(s);
        conf.decode(s);
        str16 = s.readString(900000);
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
        i76 = s.readVInt();
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
