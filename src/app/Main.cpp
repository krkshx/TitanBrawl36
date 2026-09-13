#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../logic/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../platform/audio/Music.cpp"
#include "../platform/audio/MusicLibrary.cpp"
#include "../platform/audio/SfxLibrary.cpp"
#include "../platform/fs/FileSystem.cpp"
#include "../platform/native/LoginError.cpp"
#include "../platform/native/NativeDialog.cpp"
#include "../platform/time/Clock.cpp"
#include "../platform/window/Window.cpp"
#include <cstdint>
#include <iostream>
#include <string>

static void present(LoadingScreen &screen, pc::Window &window) {
    screen.draw(window.frame(), window.width(), window.height());
    window.present();
}
static void glide(LoadingScreen &screen, pc::Window &window, float target, std::int32_t stepMs) {
    while (window.poll() && screen.progress() < target) {
        float p = screen.progress() + 0.01f;
        if (p > target) {
            p = target;
        }
        screen.setProgress(p);
        present(screen, window);
        Clock::sleepMs(stepMs);
    }
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
    std::string assetsDir = FileSystem::join(root, "assets");
    present(screen, window);
    bool textsOk = screen.loadTexts(assetsDir);
    present(screen, window);
    std::string scPath = FileSystem::join(assetsDir, FileSystem::join("sc", "loading.sc"));
    bool clipOk = screen.loadClip(scPath);
    screen.showLogo();
    screen.setProgress(0.0f);
    present(screen, window);
    std::int64_t logoUntil = Clock::nowMs() + 1500;
    while (window.poll() && Clock::nowMs() < logoUntil) {
        screen.draw(window.frame(), window.width(), window.height());
        window.present();
        Clock::sleepMs(16);
    }
    screen.showLoading();
    screen.setProgress(0.0f);
    present(screen, window);
    Music music;
    // Музыка загрузки строго по либе: themes.csv Default.ThemeMusic -> music.csv FileName.
    // Для v36 Default = Action_Western_Menu -> music/action_western_stars_01.ogg.
    std::string menuMusic = MusicLibrary::defaultMenuMusic(assetsDir);
    if (!menuMusic.empty()) {
        music.start(menuMusic, true);
    }
    glide(screen, window, 0.1f, 16);
    SupercellSWF ui;
    std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
    bool uiOk = ui.load(uiPath);
    glide(screen, window, 0.25f, 16);
    std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
    bool uiTexOk = ui.loadTexture(uiTexPath);
    glide(screen, window, 0.4f, 16);
    std::string csvPath = FileSystem::join(assetsDir, FileSystem::join("csv_logic", "characters.csv"));
    CsvTable table;
    bool csvOk = table.load(csvPath);
    ResetAccountMessage msg;
    msg.setPreset(1);
    msg.encode();
    ByteStream out;
    LogicDailyData daily;
    daily.a0_ = 10;
    daily.coins_ = 100;
    daily.encode(out);
    glide(screen, window, 0.5f, 16);
    screen.setConnecting();
    present(screen, window);
    glide(screen, window, 1.0f, 60);
    window.save(FileSystem::join(root, "frame.ppm"));
    // Сервер выключен — локальный коннект-фейл, поэтому CONNECTION_FAILED, а не LOGIN_FAILED.
    // Маппинг всех серверных кодов — в LoginError::showServerCode (задействуем когда заведём net/login).
    LoginError::showConnectionFailed(screen.localization());
    std::int64_t prev = Clock::nowMs();
    while (window.poll()) {
        std::int64_t now = Clock::nowMs();
        float dt = static_cast<float>(now - prev) / 1000.0f;
        prev = now;
        if (dt > 0.1f) {
            dt = 0.1f;
        }
        screen.update(dt);
        screen.draw(window.frame(), window.width(), window.height());
        window.present();
        Clock::sleepMs(16);
    }
    window.save(FileSystem::join(root, "frame.ppm"));
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.id() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
