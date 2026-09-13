#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../logic/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../platform/audio/Music.cpp"
#include "../platform/audio/MusicLibrary.cpp"
#include "../platform/audio/LoadingSound.cpp"
#include "../platform/audio/Sfx.cpp"
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
    // --- ЛОГО Supercell: крутим таймлайн sc_intro (100 кадров @60fps) + джингл one-shot. ---
    screen.showLogo();
    screen.setProgress(0.0f);
    Sfx::play(LoadingSound::logoJingle(assetsDir));
    {
        int frames = screen.logoFrames();
        if (frames < 1) {
            frames = 100;
        }
        std::int64_t tick = 0;
        while (window.poll()) {
            int fi = static_cast<int>(tick * 60 / 1000);
            if (fi >= frames) {
                break;
            }
            screen.setLogoFrame(fi);
            present(screen, window);
            Clock::sleepMs(16);
            tick += 16;
        }
    }
    // --- ЗАГРУЗКА: музыка загрузочного фона (sting, loop), бар — только от реальной работы. ---
    screen.showLoading();
    screen.setProgress(0.0f);
    present(screen, window);
    Music music;
    std::string loadMusic = LoadingSound::loadingMusic(assetsDir);
    if (!loadMusic.empty()) {
        music.start(loadMusic, true);
    }
    SupercellSWF ui;
    std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
    bool uiOk = ui.load(uiPath);
    screen.setProgress(0.2f);
    present(screen, window);
    std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
    bool uiTexOk = ui.loadTexture(uiTexPath);
    screen.setProgress(0.35f);
    present(screen, window);
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
    screen.setProgress(0.5f);
    present(screen, window);
    // --- КОННЕКТ: бар ЗАМИРАЕТ (никаких glide до 100%), текст — TID_CONNECTING_TO_SERVER. ---
    // (заглушка до net/login: висим на фризе короткую паузу, потом локальный фейл).
    screen.setConnecting();
    present(screen, window);
    {
        std::int64_t until = Clock::nowMs() + 2000;
        while (window.poll() && Clock::nowMs() < until) {
            present(screen, window);
            Clock::sleepMs(50);
        }
    }
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
