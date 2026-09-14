#pragma once
#include "EventData.cpp"
#include "../data/IntValueEntry.cpp"
#include "../data/CooldownEntry.cpp"
#include "../data/TimedIntValueEntry.cpp"
#include "../data/ReleaseEntry.cpp"
#include "../data/ChronosTextEntry.cpp"
#include "../notification/BaseNotification.cpp"
#include "../notification/NotificationFactory.cpp"
#include <vector>

class LogicConfData {
public:
    std::int32_t v0 = 0;
    std::vector<EventSlot> slots;
    std::vector<EventData> events;
    std::vector<std::int32_t> ints0;
    std::vector<std::int32_t> ints1;
    std::vector<std::int32_t> ints2;
    bool flag = false;
    std::vector<ReleaseEntry> releases;
    std::vector<IntValueEntry> intValues;
    std::vector<TimedIntValueEntry> timedValues;
    struct CustomEventEntry {
        std::int32_t v0 = 0;
        std::int32_t v1 = 0;
        ChronosTextEntry c[3];
    };
    std::vector<CustomEventEntry> customs;
    // Уведомления: полный разбор тел — по мере pulls; пока считаем типы,
    // базу читаем, на неизвестном типе — truncated=true (курсор стопается).
    std::vector<BaseNotification> notifications;
    bool notifTruncated = false;

    void decode(ByteStream &s) {
        v0 = s.readVInt();
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                slots.push_back(EventSlot::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                events.push_back(EventData::decode(s));
            }
        }
        for (int k = 0; k < 3; k++) {
            std::int32_t n = s.readVInt();
            std::vector<std::int32_t> *dst = k == 0 ? &ints0 : (k == 1 ? &ints1 : &ints2);
            for (std::int32_t i = 0; i < n; i++) {
                dst->push_back(s.readVInt());
            }
        }
        flag = s.readBoolean();
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                releases.push_back(ReleaseEntry::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                intValues.push_back(IntValueEntry::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                timedValues.push_back(TimedIntValueEntry::decode(s));
            }
        }
        {
            std::int32_t n = s.readVInt();
            for (std::int32_t i = 0; i < n; i++) {
                CustomEventEntry e;
                e.v0 = s.readVInt();
                e.v1 = s.readVInt();
                for (int j = 0; j < 3; j++) {
                    e.c[j] = ChronosTextEntry::decode(s);
                }
                customs.push_back(e);
            }
        }
    }
};
