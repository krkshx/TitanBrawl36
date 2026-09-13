#pragma once
#include "../../logic/data/CsvTable.cpp"
#include "../fs/FileSystem.cpp"
#include <cstdio>
#include <string>

// Резолвер SFX по либе sounds.csv: логическое имя -> assets/sfx/*.ogg.
// Плеер микширования — следующим шагом; тут только честный маппинг без хардкода.
class SfxLibrary {
public:
    // Полный путь к sfx по логическому имени ("ButtonClick2", "Menu_goback", ...).
    static std::string fileForSound(const std::string &assetsDir, const std::string &soundName) {
        if (soundName.empty()) {
            return std::string();
        }
        CsvTable sounds;
        if (!sounds.load(FileSystem::join(assetsDir, "csv_client/sounds.csv"))) {
            return std::string();
        }
        for (std::size_t r = 0; r < sounds.rows(); r++) {
            std::string name = sounds.get(r, "Name");
            if (name == "string" || name == "Name" || name.empty()) {
                continue;
            }
            if (name == soundName) {
                std::string files = sounds.get(r, "FileNames");
                std::string first = firstToken(files);
                if (first.empty()) {
                    return std::string();
                }
                std::string full = FileSystem::join(assetsDir, first);
                if (exists(full)) {
                    return full;
                }
                return std::string();
            }
        }
        return std::string();
    }

private:
    static std::string firstToken(const std::string &v) {
        std::size_t n = v.size();
        std::size_t i = 0;
        // FileNames бывает "sfx/a.ogg" или несколько через ';' — берём первый существующий токен.
        while (i < n) {
            while (i < n && (v[i] == ' ' || v[i] == ';' || v[i] == ',')) {
                i++;
            }
            if (i >= n) {
                break;
            }
            std::size_t j = i;
            while (j < n && v[j] != ';' && v[j] != ',') {
                j++;
            }
            std::string tok = v.substr(i, j - i);
            while (!tok.empty() && tok.back() == ' ') {
                tok.pop_back();
            }
            if (!tok.empty()) {
                return tok;
            }
            i = j;
        }
        return std::string();
    }
    static bool exists(const std::string &path) {
        FILE *h = std::fopen(path.c_str(), "rb");
        if (!h) {
            return false;
        }
        std::fclose(h);
        return true;
    }
};
