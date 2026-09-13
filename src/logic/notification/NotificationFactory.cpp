#pragma once
#include "BaseNotification.cpp"

struct NotificationFactory {
    static bool isKnownType(std::int32_t t) {
        if (t == '?' || t == 'F' || t == '@') {
            return true;
        }
        if (t >= 'A' && t <= 'Z') {
            return true;
        }
        return t == '[' || t == '\\' || t == ']' || t == '^';
    }
};
