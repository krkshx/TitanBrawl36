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
#include <X11/keysym.h>
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
        {
            Atom utf8 = XInternAtom(display_, "UTF8_STRING", False);
            Atom netName = XInternAtom(display_, "_NET_WM_NAME", False);
            XChangeProperty(display_, win_, netName, utf8, 8, PropModeReplace,
                            reinterpret_cast<const unsigned char *>(title.c_str()), static_cast<int>(title.size()));
        }
        wmDelete_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(display_, win_, &wmDelete_, 1);
        XSelectInput(display_, win_, ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask);
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
    // Очередь кликов в пикселях фреймбуфера. true — забрали один клик.
    bool takeClick(int &x, int &y) {
        if (clicks_.empty()) {
            return false;
        }
        x = clicks_.front().x;
        y = clicks_.front().y;
        clicks_.erase(clicks_.begin());
        return true;
    }
    // Ввод текста (экран ника): чанки UTF-8 для добавления и спецкоды:
    // -1 backspace, -2 enter. true — забрали одно событие.
    enum {
        KEY_BACKSPACE = -1,
        KEY_ENTER = -2
    };
    bool takeTextChunk(std::string &chunk) {
        if (text_.empty()) {
            return false;
        }
        chunk = text_.front();
        text_.erase(text_.begin());
        return true;
    }
    bool takeSpecialKey(int &code) {
        if (specials_.empty()) {
            return false;
        }
        code = specials_.front();
        specials_.erase(specials_.begin());
        return true;
    }
    // true один раз после ConfigureNotify с новым размером — надо перерисовать кадр.
    bool takeResized() {
        bool r = resized_;
        resized_ = false;
        return r;
    }
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
            if (e.type == ButtonPress) {
                // Окно X11 всегда размером с фреймбуфер (resize держит их вровень).
                pushClick(e.xbutton.x, e.xbutton.y);
            }
            if (e.type == KeyPress) {
                pushKey(e.xkey);
            }
            if (e.type == ConfigureNotify) {
                int nw = e.xconfigure.width;
                int nh = e.xconfigure.height;
                if (nw > 0 && nh > 0 && (nw != w_ || nh != h_)) {
                    resize(nw, nh);
                }
            }
        }
        return opened_;
#endif
    }
#if !defined(_WIN32)
    // Ресайз без битых текстур: новый framebuffer + новый XImage под него.
    void resize(int nw, int nh) {
        if (nw < 320) {
            nw = 320;
        }
        if (nh < 180) {
            nh = 180;
        }
        if (nw == w_ && nh == h_) {
            return;
        }
        w_ = nw;
        h_ = nh;
        frame_.assign(static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_), 0xFF000000u);
        if (img_ != nullptr) {
            img_->data = nullptr;
            XDestroyImage(img_);
            img_ = nullptr;
        }
        int screen = DefaultScreen(display_);
        img_ = XCreateImage(display_, DefaultVisual(display_, screen), 24, ZPixmap, 0, reinterpret_cast<char *>(frame_.data()), static_cast<unsigned>(w_), static_cast<unsigned>(h_), 32, static_cast<int>(w_ * 4));
        resized_ = true;
    }
#endif
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
        if (msg == WM_LBUTTONDOWN) {
            Window *self = reinterpret_cast<Window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            if (self) {
                self->pushClientClick(hwnd, static_cast<int>(static_cast<short>(LOWORD(lp))), static_cast<int>(static_cast<short>(HIWORD(lp))));
            }
        }
        if (msg == WM_KEYDOWN) {
            Window *self = reinterpret_cast<Window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            if (self) {
                if (wp == VK_BACK) {
                    self->specials_.push_back(KEY_BACKSPACE);
                } else if (wp == VK_RETURN) {
                    self->specials_.push_back(KEY_ENTER);
                }
            }
        }
        if (msg == WM_CHAR) {
            Window *self = reinterpret_cast<Window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            if (self && wp >= 32 && wp != 127) {
                // BMP -> UTF-8 (латиница/кириллица покрыты; суррогаты склеиваем грубо).
                char out[4];
                int n = 0;
                if (wp < 0x80) {
                    out[0] = static_cast<char>(wp);
                    n = 1;
                } else if (wp < 0x800) {
                    out[0] = static_cast<char>(0xC0 | (wp >> 6));
                    out[1] = static_cast<char>(0x80 | (wp & 0x3F));
                    n = 2;
                } else {
                    out[0] = static_cast<char>(0xE0 | (wp >> 12));
                    out[1] = static_cast<char>(0x80 | ((wp >> 6) & 0x3F));
                    out[2] = static_cast<char>(0x80 | (wp & 0x3F));
                    n = 3;
                }
                self->text_.emplace_back(out, static_cast<std::size_t>(n));
            }
        }
        return DefWindowProcA(hwnd, msg, wp, lp);
    }
    // Клик в клиентских пикселях -> во фреймбуфер (клиент меньше окна из-за рамки).
    void pushClientClick(HWND hwnd, int cx, int cy) {
        RECT rc;
        if (!GetClientRect(hwnd, &rc)) {
            return;
        }
        int cw = rc.right - rc.left;
        int ch = rc.bottom - rc.top;
        if (cw <= 0 || ch <= 0) {
            return;
        }
        pushClick(cx * w_ / cw, cy * h_ / ch);
    }
#endif
private:
    struct Click {
        int x = 0;
        int y = 0;
    };
    void pushClick(int fx, int fy) {
        if (fx < 0 || fy < 0 || fx >= w_ || fy >= h_) {
            return;
        }
        Click c;
        c.x = fx;
        c.y = fy;
        clicks_.push_back(c);
    }
#if !defined(_WIN32)
    // KeyPress -> очередь: backspace/enter спецкодами, остальное чанками UTF-8.
    void pushKey(XKeyEvent &kev) {
        char buf[32];
        KeySym ks = NoSymbol;
        int n = XLookupString(&kev, buf, sizeof(buf), &ks, nullptr);
        if (ks == XK_BackSpace) {
            specials_.push_back(KEY_BACKSPACE);
            return;
        }
        if (ks == XK_Return || ks == XK_KP_Enter) {
            specials_.push_back(KEY_ENTER);
            return;
        }
        if (ks == XK_Escape || n <= 0) {
            return;
        }
        // Фильтр управляющих символов, переводы строк отдельно не нужны.
        if (n == 1 && (buf[0] < 32 || buf[0] == 127)) {
            return;
        }
        text_.emplace_back(buf, static_cast<std::size_t>(n));
    }
#endif
    int w_ = 0;
    int h_ = 0;
    std::string title_;
    std::vector<std::uint32_t> frame_;
    std::vector<Click> clicks_;
    std::vector<std::string> text_;
    std::vector<int> specials_;
    bool opened_ = false;
    bool resized_ = false;
#if defined(_WIN32)
    HWND hwnd_ = nullptr;
#else
    Display *display_ = nullptr;
    ::Window win_ = 0;
    GC gc_ = nullptr;
    XImage *img_ = nullptr;
    Atom wmDelete_ = 0;
#endif
};
}
