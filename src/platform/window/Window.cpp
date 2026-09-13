#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

namespace pc {
class Window {
public:
    bool open(int w, int h, const std::string &title) {
        w_ = w;
        h_ = h;
        title_ = title;
        frame_.assign(static_cast<std::size_t>(w) * static_cast<std::size_t>(h), 0xFF000000u);
#if defined(_WIN32)
        HINSTANCE inst = GetModuleHandleA(nullptr);
        WNDCLASSA wc = {};
        wc.lpfnWndProc = Window::proc;
        wc.hInstance = inst;
        wc.lpszClassName = "TitanBrawl36";
        RegisterClassA(&wc);
        hwnd_ = CreateWindowExA(0, "TitanBrawl36", title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, w + 16, h + 39, nullptr, nullptr, inst, this);
        opened_ = (hwnd_ != nullptr);
        return opened_;
#else
        display_ = XOpenDisplay(nullptr);
        if (display_ == nullptr) {
            return false;
        }
        int screen = DefaultScreen(display_);
        win_ = XCreateSimpleWindow(display_, RootWindow(display_, screen), 0, 0, static_cast<unsigned>(w), static_cast<unsigned>(h), 1, BlackPixel(display_, screen), WhitePixel(display_, screen));
        XStoreName(display_, win_, title.c_str());
        wmDelete_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(display_, win_, &wmDelete_, 1);
        XSelectInput(display_, win_, ExposureMask | KeyPressMask | StructureNotifyMask);
        XMapWindow(display_, win_);
        gc_ = XCreateGC(display_, win_, 0, nullptr);
        img_ = XCreateImage(display_, DefaultVisual(display_, screen), 24, ZPixmap, 0, reinterpret_cast<char *>(frame_.data()), static_cast<unsigned>(w), static_cast<unsigned>(h), 32, static_cast<int>(w * 4));
        opened_ = true;
        return true;
#endif
    }
    void clear(std::uint32_t color) {
        for (auto &p : frame_) {
            p = color;
        }
    }
    std::vector<std::uint32_t> &frame() { return frame_; }
    int width() const { return w_; }
    int height() const { return h_; }
    bool opened() const { return opened_; }
    bool poll() {
#if defined(_WIN32)
        MSG msg;
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                opened_ = false;
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        return opened_;
#else
        if (display_ == nullptr) {
            return false;
        }
        while (XPending(display_)) {
            XEvent e;
            XNextEvent(display_, &e);
            if (e.type == ClientMessage) {
                if (static_cast<Atom>(e.xclient.data.l[0]) == wmDelete_) {
                    opened_ = false;
                    return false;
                }
            }
            if (e.type == DestroyNotify) {
                opened_ = false;
                return false;
            }
        }
        return opened_;
#endif
    }
    void present() {
#if defined(_WIN32)
        if (hwnd_ != nullptr) {
            HDC dc = GetDC(hwnd_);
            BITMAPINFO bi = {};
            bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
            bi.bmiHeader.biWidth = w_;
            bi.bmiHeader.biHeight = -h_;
            bi.bmiHeader.biPlanes = 1;
            bi.bmiHeader.biBitCount = 32;
            bi.bmiHeader.biCompression = BI_RGB;
            StretchDIBits(dc, 0, 0, w_, h_, 0, 0, w_, h_, frame_.data(), &bi, DIB_RGB_COLORS, SRCCOPY);
            ReleaseDC(hwnd_, dc);
        }
#else
        if (display_ != nullptr && img_ != nullptr) {
            img_->data = reinterpret_cast<char *>(frame_.data());
            XPutImage(display_, win_, gc_, img_, 0, 0, 0, 0, static_cast<unsigned>(w_), static_cast<unsigned>(h_));
            XFlush(display_);
        }
#endif
    }
    bool save(const std::string &path) const {
        std::ofstream f(path, std::ios::binary);
        if (!f) {
            return false;
        }
        f << "P6\n" << w_ << " " << h_ << "\n255\n";
        for (std::uint32_t p : frame_) {
            unsigned char r = static_cast<unsigned char>((p >> 16) & 0xFF);
            unsigned char g = static_cast<unsigned char>((p >> 8) & 0xFF);
            unsigned char b = static_cast<unsigned char>(p & 0xFF);
            f.write(reinterpret_cast<const char *>(&r), 1);
            f.write(reinterpret_cast<const char *>(&g), 1);
            f.write(reinterpret_cast<const char *>(&b), 1);
        }
        return static_cast<bool>(f);
    }
#if defined(_WIN32)
    static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        if (msg == WM_DESTROY) {
            PostQuitMessage(0);
            return 0;
        }
        if (msg == WM_CREATE) {
            CREATESTRUCTA *cs = reinterpret_cast<CREATESTRUCTA *>(lp);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        }
        return DefWindowProcA(hwnd, msg, wp, lp);
    }
#endif
private:
    int w_ = 0;
    int h_ = 0;
    std::string title_;
    std::vector<std::uint32_t> frame_;
    bool opened_ = false;
#if defined(_WIN32)
    void *hwnd_ = nullptr;
#else
    Display *display_ = nullptr;
    ::Window win_ = 0;
    GC gc_ = nullptr;
    XImage *img_ = nullptr;
    Atom wmDelete_ = 0;
#endif
};
}
