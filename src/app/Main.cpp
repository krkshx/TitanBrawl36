#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../titan/ui/UiButton.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../titan/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../fmod/Music.cpp"
#include "../fmod/MusicLibrary.cpp"
#include "../fmod/LoadingSound.cpp"
#include "../fmod/Sfx.cpp"
#include "../fmod/SfxLibrary.cpp"
#include "../helpers/FileSystem.cpp"
#include "../platform/native/LoginError.cpp"
#include "../platform/native/NativeDialog.cpp"
#include "../helpers/Clock.cpp"
#include "../platform/window/Window.cpp"
#include <cstdint>
#include <iostream>
#include <string>

static void present(LoadingScreen &screen, UiButton *button, pc::Window &window) {
    screen.draw(window.frame(), window.width(), window.height());
    if (button) {
        button->draw(window.frame(), window.width(), window.height());
    }
    window.present();
}
// Клики по кнопке входа: статус-текст становится заданным. true — попали по кнопке.
static bool drainEntryClicks(pc::Window &window, UiButton *button, LoadingScreen &screen) {
    if (!button || !button->bound()) {
        return false;
    }
    bool hit = false;
    int cx = 0;
    int cy = 0;
    while (window.takeClick(cx, cy)) {
        if (button->hit(cx, cy, window.width(), window.height())) {
            screen.setStatusOverride("pmdrk loh...");
            hit = true;
        }
    }
    return hit;
}

int main(int argc, char **argv) {
    std::string root = ".";
    if (argc > 1) {
        root = argv[1];
    }
    pc::Window window;
    if (!window.open(848, 480, "Laser")) {
        std::cout << "window=0" << "\n";
        return 4;
    }
    LoadingScreen screen;
    UiButton entryButton;
    bool buttonOk = false;
    std::string assetsDir = FileSystem::join(root, "assets");
    present(screen, &entryButton, window);
    bool textsOk = screen.loadTexts(assetsDir);
    present(screen, &entryButton, window);
    std::string scPath = FileSystem::join(assetsDir, FileSystem::join("sc", "loading.sc"));
    bool clipOk = screen.loadClip(scPath);


    if (!clipOk) {
        // Без loading.sc экран остаётся чёрным на всех стадиях — сказать прямо, а не молчать.
        NativeDialog::show("Missing assets",
            "Could not load " + scPath + ".\nCopy the decompiled assets/ folder (sc/, csv_logic/, sfx/) next to the game and run again.",
            "OK");
    }
    // === ПОЛНЫЙ БУТ (цикл): ЛОГО → ЗАГРУЗКА → КОННЕКТ → диалог.
    // "Попробовать снова" крутит всё с самого начала: сначала лого, потом лоадинг, как в ориге.

    SupercellSWF ui;
    CsvTable table;
    ResetAccountMessage msg;
    ByteStream out;
    LogicDailyData daily;
    bool uiOk = false;
    bool uiTexOk = false;
    bool csvOk = false;
    while (window.poll()) {
        screen.showLogo();
        screen.setProgress(0.0f);
        Sfx::play(LoadingSound::logoJingle(assetsDir));
        {
            int frames = screen.logoFrames();
            if (frames < 1) {
                frames = 100;
            }
            std::int64_t t0 = Clock::nowMs();
            for (;;) {
                if (!window.poll()) {
                    break;
                }
                drainEntryClicks(window, &entryButton, screen);
                int fi = static_cast<int>((Clock::nowMs() - t0) * 60 / 1000);
                if (fi >= frames) {
                    break;
                }
                screen.setLogoFrame(fi);
                present(screen, &entryButton, window);
            }
        }
        if (!window.poll()) {
            break;
        }
        screen.showLoading();
        screen.setProgress(0.0f);
        present(screen, &entryButton, window);
        Music music;
        std::string loadMusic = LoadingSound::loadingMusic(assetsDir);
        if (!loadMusic.empty()) {
            music.start(loadMusic, false);
        }
        ui = SupercellSWF();
        std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
        uiOk = ui.load(uiPath);
        screen.setProgress(0.2f);
        present(screen, &entryButton, window);
        std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
        uiTexOk = ui.loadTexture(uiTexPath, [&](int done, int total) {
            float f = total > 0 ? static_cast<float>(done) / static_cast<float>(total) : 1.0f;
            screen.setProgress(0.2f + f * 0.15f);
            present(screen, &entryButton, window);
        });
        // Кнопка входа: первый экспорт ui.sc с "button" в имени, текстуры уже на месте.
        buttonOk = entryButton.bind(&ui, "button");
        screen.setProgress(0.35f);
        present(screen, &entryButton, window);
        std::string csvPath = FileSystem::join(assetsDir, FileSystem::join("csv_logic", "characters.csv"));
        table = CsvTable();
        csvOk = table.load(csvPath);
        msg = ResetAccountMessage();
        msg.setPreset(1);
        msg.encode();
        out = ByteStream();
        daily = LogicDailyData();
        daily.a0_ = 10;
        daily.coins_ = 100;
        daily.encode(out);
        screen.setProgress(0.5f);
        present(screen, &entryButton, window);
        screen.setConnecting();
        present(screen, &entryButton, window);
        {
            std::int64_t until = Clock::nowMs() + 2000;
            while (window.poll() && Clock::nowMs() < until) {
                // Текст мог смениться кликом — тогда кадр перерисовать, а не blit.
                if (drainEntryClicks(window, &entryButton, screen) || window.takeResized()) {
                    present(screen, &entryButton, window);
                } else {
                    window.present();
                }
                Clock::sleepMs(50);
            }
        }
        if (!window.poll()) {
            break;
        }
        window.save(FileSystem::join(root, "frame.ppm"));
        bool retry = LoginError::askConnectionFailed(screen.localization());
        if (!retry) {
            break;
        }
    }
    window.save(FileSystem::join(root, "frame.ppm"));
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.getMessageType() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " button=" << (buttonOk ? entryButton.assetName() : "-") << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
