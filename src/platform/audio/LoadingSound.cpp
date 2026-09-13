#pragma once
#include "../../logic/data/CsvTable.cpp"
#include "../fs/FileSystem.cpp"
#include <cstdio>
#include <string>

// Звуки бут-стадий захардкожены в ориг-бинаре (строк в sounds.csv/music.csv у них НЕТ):
//   - sfx/supercell_jingle.ogg — джингл лого Supercell (~2с, one-shot);
//   - *_load_01.ogg — музыка загрузочного фона (sting).
// Арты loading.sc в этой сборке — dino beach (Velocirapids), ему соответствует
// sfx/summer_dino_load_01.ogg. Дальше фолбэки по наличию файлов.
class LoadingSound {
public:
    static std::string logoJingle(const std::string &assetsDir) {
        std::string p = FileSystem::join(assetsDir, "sfx/supercell_jingle.ogg");
        return exists(p) ? p : std::string();
    }

    static std::string loadingMusic(const std::string &assetsDir) {
        const char *chain[] = {
            "sfx/summer_dino_load_01.ogg",
            "sfx/action_western_load_01.ogg",
            "sfx/laser_load_01.ogg",
            nullptr
        };
        for (int i = 0; chain[i]; i++) {
            std::string p = FileSystem::join(assetsDir, chain[i]);
            if (exists(p)) {
                return p;
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
