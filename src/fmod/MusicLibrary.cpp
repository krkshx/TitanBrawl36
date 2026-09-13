#pragma once
#include "../titan/data/CsvTable.cpp"
#include "../helpers/FileSystem.cpp"
#include <cstdio>
#include <string>
#include <vector>

// Резолвер музыки по либе (как в ориге, своего не добавляем):
// themes.csv (Default -> ThemeMusic) -> music.csv (ThemeMusic -> FileName).
class MusicLibrary {
public:
    // Полный путь к меню-музыке для темы Default (загрузка до логина всегда Default).
    static std::string defaultMenuMusic(const std::string &assetsDir) {
        std::string themeMusic = themeMusicFor(assetsDir, "Default");
        if (themeMusic.empty()) {
            themeMusic = "Action_Western_Menu";
        }
        std::string path = fileForMusic(assetsDir, themeMusic);
        if (!path.empty()) {
            return path;
        }
        // Фолбэк-цепочка по либе, если файл темы отсутствует в сборке.
        const char *chain[] = {
            "Action_Western_Menu", "Punk_Brawl_Menu",
            "Summer_Brawl_Menu_Muzak", "Cnymenu_music", nullptr
        };
        for (int i = 0; chain[i]; i++) {
            if (themeMusic == chain[i]) {
                continue;
            }
            path = fileForMusic(assetsDir, chain[i]);
            if (!path.empty()) {
                return path;
            }
        }
        return std::string();
    }

    static std::string themeMusicFor(const std::string &assetsDir, const std::string &themeName) {
        CsvTable themes;
        if (!themes.load(FileSystem::join(assetsDir, "csv_logic/themes.csv"))) {
            return std::string();
        }
        for (std::size_t r = 0; r < themes.rows(); r++) {
            std::string name = themes.get(r, "Name");
            if (name.empty() || name == "string" || name == "Name") {
                continue;
            }
            if (name == themeName) {
                return themes.get(r, "ThemeMusic");
            }
        }
        return std::string();
    }

    // musicName напр. "Action_Western_Menu" -> "assets/music/action_western_stars_01.ogg" (если файл есть).
    static std::string fileForMusic(const std::string &assetsDir, const std::string &musicName) {
        if (musicName.empty()) {
            return std::string();
        }
        CsvTable music;
        if (!music.load(FileSystem::join(assetsDir, "csv_client/music.csv"))) {
            return std::string();
        }
        std::string file;
        std::string fallback;
        for (std::size_t r = 0; r < music.rows(); r++) {
            std::string name = music.get(r, "Name");
            if (name == "string" || name == "Name") {
                continue;
            }
            if (name == musicName) {
                file = music.get(r, "FileName");
                fallback = music.get(r, "FallBackMusic");
                break;
            }
        }
        if (!file.empty()) {
            std::string full = FileSystem::join(assetsDir, file);
            if (exists(full)) {
                return full;
            }
        }
        // FallBackMusic из той же строки (напр. Brawzaar_menu_muzak -> Action_Western_Menu).
        if (!fallback.empty() && fallback != musicName) {
            std::string fb = fileForMusic(assetsDir, fallback);
            if (!fb.empty()) {
                return fb;
            }
        }
        return std::string();
    }

private:
    static bool exists(const std::string &path) {
        FILE *h = std::fopen(path.c_str(), "rb");
        if (!h) {
            return false;
        }
        std::fclose(h);
        return true;
    }
};
