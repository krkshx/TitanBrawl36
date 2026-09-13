#pragma once
#include "NativeDialog.cpp"
#include "../../logic/data/Localization.cpp"
#include <string>

// Маппинг error-type -> TID-набор (как в ориге, своего не добавляем).
// Покрывает все TID_ERROR_POP_UP_* из localization/*.csv.
class LoginError {
public:
    enum Code {
        kConnectionFailed = 0,   // нет сети / сервер выключен — наш текущий кейс
        kLoginFailed = 1,        // generic fallback для неизвестных кодов сервера
        kOutOfSync = 2,
        kMaintenance = 9,
        kBanned = 10,
        kWrongVersion = 8,
        kConnectionLost = 11,
        kLoggedFromAnotherDevice = 12,
        kLoginNotAllowed = 13
    };

    static void showConnectionFailed(const Localization &texts) {
        show(texts,
             "TID_ERROR_POP_UP_CONNECTION_FAILED_TITLE", "Connection error",
             "TID_ERROR_POP_UP_CONNECTION_FAILED", "Unable to connect with the server. Check your internet connection and try again.",
             "TID_ERROR_POP_UP_CONNECTION_FAILED_BUTTON", "Try again");
    }

    static void showLoginFailed(const Localization &texts) {
        show(texts,
             "TID_ERROR_POP_UP_LOGIN_FAILED_TITLE", "Login failed",
             "TID_ERROR_POP_UP_LOGIN_FAILED", "Please try again later.",
             "TID_ERROR_POP_UP_LOGIN_FAILED_BUTTON", "Try again");
    }

    // Маппинг серверного LoginFailed errorCode -> попап. Неизвестный код -> generic LoginFailed.
    // Коды уточним когда заведём net/login; пока маппим известные семейства из TIDs.
    static void showServerCode(int code, const Localization &texts) {
        switch (code) {
            case 8:
                show(texts,
                     "TID_ERROR_POP_UP_WRONG_CLIENT_VERSION_TITLE", "Update is available!",
                     "TID_ERROR_POP_UP_WRONG_CLIENT_VERSION", "Good news! A new version of Brawl Stars is available.",
                     "TID_ERROR_POP_UP_WRONG_CLIENT_VERSION_BUTTON", "Update");
                return;
            case 9:
                show(texts,
                     "TID_ERROR_POP_UP_SERVER_MAINTENANCE_TITLE", "Maintenance break",
                     "TID_ERROR_POP_UP_SERVER_MAINTENANCE", "Sorry, the servers are under maintenance. Please try again later.",
                     "TID_ERROR_POP_UP_SERVER_MAINTENANCE_BUTTON", "Try again");
                return;
            case 10:
                show(texts,
                     "TID_ERROR_POP_UP_BANNED_TITLE", "You have been banned!",
                     "TID_ERROR_POP_UP_BANNED", "You have been banned temporarily",
                     "TID_ERROR_POP_UP_BANNED_BUTTON", "Okay");
                return;
            case 11:
                show(texts,
                     "TID_ERROR_POP_UP_CONNECTION_LOST_TITLE", "Connection lost",
                     "TID_ERROR_POP_UP_CONNECTION_LOST", "Please try logging in again!",
                     "TID_ERROR_POP_UP_CONNECTION_LOST_BUTTON", "Retry login");
                return;
            case 12:
                show(texts,
                     "TID_ERROR_POP_UP_LOGGED_FROM_ANOTHER_DEVICE_TITLE", "Connection lost",
                     "TID_ERROR_POP_UP_LOGGED_FROM_ANOTHER_DEVICE", "Another device is connecting to this game.",
                     "TID_ERROR_POP_UP_LOGGED_FROM_ANOTHER_DEVICE_BUTTON", "Reload");
                return;
            case 13:
                show(texts,
                     "TID_ERROR_POP_UP_LOGIN_NOT_ALLOWED_TITLE", "Login failed",
                     "TID_ERROR_POP_UP_LOGIN_NOT_ALLOWED", "Brawl Stars Beta is not yet available in your country. Check back later!",
                     "TID_ERROR_POP_UP_LOGIN_NOT_ALLOWED_BUTTON", "Okay");
                return;
            case 2:
                show(texts,
                     "TID_ERROR_POP_UP_OUT_OF_SYNC_TITLE", "Error!",
                     "TID_ERROR_POP_UP_OUT_OF_SYNC", "Client and server are out of sync!",
                     "TID_ERROR_POP_UP_OUT_OF_SYNC_BUTTON", "Reload game");
                return;
            case 0:
                showConnectionFailed(texts);
                return;
            case 1:
            default:
                showLoginFailed(texts);
                return;
        }
    }

private:
    static void show(const Localization &texts,
                     const std::string &titleTid, const std::string &titleFb,
                     const std::string &msgTid, const std::string &msgFb,
                     const std::string &btnTid, const std::string &btnFb) {
        // Достаём локализованный текст через либу (LANG -> ru.csv / texts.csv), как в ориге.
        NativeDialog::show(texts.text(titleTid, titleFb),
                           texts.text(msgTid, msgFb),
                           texts.text(btnTid, btnFb));
    }
};
