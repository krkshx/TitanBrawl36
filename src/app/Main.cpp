#pragma once
#include "../titan/core/ByteStream.cpp"
#include "../titan/ui/LoadingScreen.cpp"
#include "../logic/player/LogicDailyData.cpp"
#include "../titan/data/CsvTable.cpp"
#include "../net/account/ResetAccountMessage.cpp"
#include "../net/core/Messaging.cpp"
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
#include <vector>

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
                int fi = static_cast<int>((Clock::nowMs() - t0) * 60 / 1000);
                if (fi >= frames) {
                    break;
                }
                screen.setLogoFrame(fi);
                present(screen, window);
            }
        }
        if (!window.poll()) {
            break;
        }
        screen.showLoading();
        screen.setProgress(0.0f);
        present(screen, window);
        Music music;
        std::string loadMusic = LoadingSound::loadingMusic(assetsDir);
        if (!loadMusic.empty()) {
            music.start(loadMusic, false);
        }
        ui = SupercellSWF();
        std::string uiPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui.sc"));
        uiOk = ui.load(uiPath);
        screen.setProgress(0.2f);
        present(screen, window);
        std::string uiTexPath = FileSystem::join(assetsDir, FileSystem::join("sc", "ui_tex.sc"));
        uiTexOk = ui.loadTexture(uiTexPath, [&](int done, int total) {
            float f = total > 0 ? static_cast<float>(done) / static_cast<float>(total) : 1.0f;
            screen.setProgress(0.2f + f * 0.15f);
            present(screen, window);
        });
        screen.setProgress(0.35f);
        present(screen, window);
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
        present(screen, window);
        screen.setConnecting();
        present(screen, window);
        {
            ClientMessaging net;
            bool netOk = false;
            std::string netInfo = "offline";
            if (net.connect(5000)) {
                std::uint8_t seedBuf[4];
                randomBytesTrue(seedBuf, 4);
                std::int32_t seed = static_cast<std::int32_t>(seedBuf[0] | (seedBuf[1] << 8) | (seedBuf[2] << 16) | (seedBuf[3] << 24));
                std::vector<std::uint8_t> token;
                if (net.sendClientHello(seed) && net.receiveServerHello(token)) {
                    LoginMessage login;
                    login.setAccountId(0);
                    login.setPassToken("");
                    login.setVersion(36, 218);
                    login.setRndKey(randomIntTrue());
                    if (net.sendPepperLogin(login, token)) {
                        PiranhaMessage *first = net.receivePepperResponse(true);
                        if (first != nullptr) {
                            netInfo = std::string("first=") + std::to_string(first->getMessageType());
                            if (first->getMessageType() == 26007) {
                                PiranhaMessage *second = net.receiveNext(8000);
                                if (second != nullptr) {
                                    netInfo += std::string(" second=") + std::to_string(second->getMessageType());
                                    if (second->getMessageType() == 24101) {
                                        auto *home = static_cast<OwnHomeDataMessage *>(second);
                                        netInfo += std::string(" homeBytes=") + std::to_string(home->raw().size());
                                    }
                                    if (second->getMessageType() == 20104) {
                                        netOk = true;
                                    }
                                    delete second;
                                } else {
                                    netInfo += " second=timeout";
                                }
                            } else if (first->getMessageType() == 20104) {
                                netOk = true;
                            }
                            delete first;
                        } else {
                            netInfo = std::string("pepper-response-fail: ") + net.lastError();
                        }
                    } else {
                        netInfo = std::string("pepper-login-fail: ") + net.lastError();
                    }
                } else {
                    netInfo = std::string("hello-fail: ") + net.lastError();
                }
            } else {
                netInfo = std::string("connect-fail: ") + net.lastError();
            }
            std::cout << "net=" << (netOk ? 1 : 0) << " " << netInfo << " account=" << 0 << "\n";
            std::int64_t until = Clock::nowMs() + 1500;
            while (window.poll() && Clock::nowMs() < until) {
                if (window.takeResized()) {
                    present(screen, window);
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
    std::cout << "clip=" << clipOk << " texts=" << textsOk << " csv=" << csvOk << " rows=" << table.rows() << " msg=" << msg.getMessageType() << " ui=" << uiOk << uiTexOk << " clips=" << ui.clips.size() << " text=" << screen.statusText() << " stage=loading" << "\n";
    return 0;
}
