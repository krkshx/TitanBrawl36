#pragma once
#include "NativeDialog.cpp"
#include "../../titan/data/Localization.cpp"
#include <string>


class LoginError {
public:
    enum Code {
        kConnectionFailed = 0,   
        kLoginFailed = 1,       
        kOutOfSync = 2,
        kMaintenance = 9,
        kBanned = 10,
        kWrongVersion = 8,
        kConnectionLost = 11,
        kLoggedFromAnotherDevice = 12,
        kLoginNotAllowed = 13
    };

    static bool askConnectionFailed(const Localization &texts) {
        return ask(texts,
             "TID_ERROR_POP_UP_CONNECTION_FAILED_TITLE", "Connection error",
             "TID_ERROR_POP_UP_CONNECTION_FAILED", "Unable to connect with the server. Check your internet connection and try again.",
             "TID_ERROR_POP_UP_CONNECTION_FAILED_BUTTON", "Try again");
    }

    static bool askLoginFailed(const Localization &texts) {
        return ask(texts,
             "TID_ERROR_POP_UP_LOGIN_FAILED_TITLE", "Login failed",
             "TID_ERROR_POP_UP_LOGIN_FAILED", "Please try again later.",
             "TID_ERROR_POP_UP_LOGIN_FAILED_BUTTON", "Try again");
    }


    static bool askServerCode(int code, const Localization &texts) {
        switch (code) {
            case 8:
                return ask(texts,
                     "TID_ERROR_POP_UP_WRONG_CLIENT_VERSION_TITLE", "Update is available!",
                     "TID_ERROR_POP_UP_WRONG_CLIENT_VERSION", "Good news! A new version of Brawl Stars is available.",
                     "TID_ERROR_POP_UP_WRONG_CLIENT_VERSION_BUTTON", "Update");
            case 9:
                return ask(texts,
                     "TID_ERROR_POP_UP_SERVER_MAINTENANCE_TITLE", "Maintenance break",
                     "TID_ERROR_POP_UP_SERVER_MAINTENANCE", "Sorry, the servers are under maintenance. Please try again later.",
                     "TID_ERROR_POP_UP_SERVER_MAINTENANCE_BUTTON", "Try again");
            case 10:
                return ask(texts,
                     "TID_ERROR_POP_UP_BANNED_TITLE", "You have been banned!",
                     "TID_ERROR_POP_UP_BANNED", "You have been banned temporarily",
                     "TID_ERROR_POP_UP_BANNED_BUTTON", "Okay");
            case 11:
                return ask(texts,
                     "TID_ERROR_POP_UP_CONNECTION_LOST_TITLE", "Connection lost",
                     "TID_ERROR_POP_UP_CONNECTION_LOST", "Please try logging in again!",
                     "TID_ERROR_POP_UP_CONNECTION_LOST_BUTTON", "Retry login");
            case 12:
                return ask(texts,
                     "TID_ERROR_POP_UP_LOGGED_FROM_ANOTHER_DEVICE_TITLE", "Connection lost",
                     "TID_ERROR_POP_UP_LOGGED_FROM_ANOTHER_DEVICE", "Another device is connecting to this game.",
                     "TID_ERROR_POP_UP_LOGGED_FROM_ANOTHER_DEVICE_BUTTON", "Reload");
            case 13:
                return ask(texts,
                     "TID_ERROR_POP_UP_LOGIN_NOT_ALLOWED_TITLE", "Login failed",
                     "TID_ERROR_POP_UP_LOGIN_NOT_ALLOWED", "Brawl Stars Beta is not yet available in your country. Check back later!",
                     "TID_ERROR_POP_UP_LOGIN_NOT_ALLOWED_BUTTON", "Okay");
            case 2:
                return ask(texts,
                     "TID_ERROR_POP_UP_OUT_OF_SYNC_TITLE", "Error!",
                     "TID_ERROR_POP_UP_OUT_OF_SYNC", "Client and server are out of sync!",
                     "TID_ERROR_POP_UP_OUT_OF_SYNC_BUTTON", "Reload game");
            case 0:
                return askConnectionFailed(texts);
            case 1:
            default:
                return askLoginFailed(texts);
        }
    }

private:
    static bool ask(const Localization &texts,
                     const std::string &titleTid, const std::string &titleFb,
                     const std::string &msgTid, const std::string &msgFb,
                     const std::string &btnTid, const std::string &btnFb) {
        // Достаём локализованный текст через либу (LANG -> ru.csv / texts.csv), как в ориге.
        return NativeDialog::ask(texts.text(titleTid, titleFb),
                           texts.text(msgTid, msgFb),
                           texts.text(btnTid, btnFb));
    }
};
