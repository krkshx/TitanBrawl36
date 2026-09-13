#pragma once
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <string>
#include <thread>
#include <vector>
#if defined(TITAN_HAS_AUDIO)
#include <vorbis/vorbisfile.h>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#else
#include <pulse/simple.h>
#endif
#endif

class Music {
public:
    ~Music() {
        stop();
    }
    bool start(const std::string &path, bool loop) {
        stop();
#if defined(TITAN_HAS_AUDIO)
        path_ = path;
        loop_ = loop;
        quit_ = false;
        playing_ = false;
        thread_ = std::thread(&Music::run, this);
        return true;
#else
        (void)path;
        (void)loop;
        return false;
#endif
    }
    void stop() {
#if defined(TITAN_HAS_AUDIO)
        quit_ = true;
        if (thread_.joinable()) {
            thread_.join();
        }
        playing_ = false;
#endif
    }
    bool playing() const {
#if defined(TITAN_HAS_AUDIO)
        return playing_;
#else
        return false;
#endif
    }
private:
#if defined(TITAN_HAS_AUDIO)
    void run() {
        FILE *handle = fopen(path_.c_str(), "rb");
        if (!handle) {
            return;
        }
        OggVorbis_File vf;
        if (ov_open(handle, &vf, nullptr, 0) < 0) {
            fclose(handle);
            return;
        }
        vorbis_info *vi = ov_info(&vf, -1);
        if (!vi || (vi->channels != 1 && vi->channels != 2)) {
            ov_clear(&vf);
            return;
        }
        int rate = vi->rate;
        int channels = vi->channels;
        std::vector<char> pcm;
        pcm.reserve(1 << 20);
        char buf[8192];
        int stream = 0;
        for (;;) {
            long got = ov_read(&vf, buf, sizeof(buf), 0, 2, 1, &stream);
            if (got <= 0) {
                break;
            }
            pcm.insert(pcm.end(), buf, buf + got);
        }
        ov_clear(&vf);
        if (pcm.empty()) {
            return;
        }
#if defined(_WIN32)
        playWave(pcm, rate, channels);
#else
        playPulse(pcm, rate, channels);
#endif
    }
#if defined(_WIN32)
    void playWave(const std::vector<char> &pcm, int rate, int channels) {
        WAVEFORMATEX fmt = {};
        fmt.wFormatTag = WAVE_FORMAT_PCM;
        fmt.nChannels = static_cast<WORD>(channels);
        fmt.nSamplesPerSec = static_cast<DWORD>(rate);
        fmt.wBitsPerSample = 16;
        fmt.nBlockAlign = static_cast<WORD>(channels * 2);
        fmt.nAvgBytesPerSec = static_cast<DWORD>(rate * channels * 2);
        HWAVEOUT out = nullptr;
        if (waveOutOpen(&out, WAVE_MAPPER, &fmt, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
            return;
        }
        playing_ = true;
        const std::size_t chunk = 1 << 16;
        std::size_t pos = 0;
        while (!quit_) {
            if (pos >= pcm.size()) {
                if (!loop_) {
                    break;
                }
                pos = 0;
            }
            std::size_t n = pcm.size() - pos;
            if (n > chunk) {
                n = chunk;
            }
            WAVEHDR hdr = {};
            hdr.lpData = const_cast<LPSTR>(pcm.data() + pos);
            hdr.dwBufferLength = static_cast<DWORD>(n);
            if (waveOutPrepareHeader(out, &hdr, sizeof(hdr)) != MMSYSERR_NOERROR) {
                break;
            }
            if (waveOutWrite(out, &hdr, sizeof(hdr)) != MMSYSERR_NOERROR) {
                waveOutUnprepareHeader(out, &hdr, sizeof(hdr));
                break;
            }
            while (!quit_ && !(hdr.dwFlags & WHDR_DONE)) {
                Sleep(10);
            }
            waveOutUnprepareHeader(out, &hdr, sizeof(hdr));
            if (quit_) {
                waveOutReset(out);
                break;
            }
            pos += n;
        }
        waveOutClose(out);
        playing_ = false;
    }
#else
    void playPulse(const std::vector<char> &pcm, int rate, int channels) {
        pa_sample_spec ss;
        ss.format = PA_SAMPLE_S16LE;
        ss.rate = static_cast<std::uint32_t>(rate);
        ss.channels = static_cast<std::uint8_t>(channels);
        int err = 0;
        pa_simple *dev = pa_simple_new(nullptr, "TitanBrawl", PA_STREAM_PLAYBACK, nullptr, "music", &ss, nullptr, nullptr, &err);
        if (!dev) {
            return;
        }
        playing_ = true;
        const std::size_t chunk = 1 << 15;
        std::size_t pos = 0;
        while (!quit_) {
            if (pos >= pcm.size()) {
                if (!loop_) {
                    break;
                }
                pos = 0;
            }
            std::size_t n = pcm.size() - pos;
            if (n > chunk) {
                n = chunk;
            }
            if (pa_simple_write(dev, pcm.data() + pos, n, &err) < 0) {
                break;
            }
            pos += n;
        }
        pa_simple_drain(dev, nullptr);
        pa_simple_free(dev);
        playing_ = false;
    }
#endif
    std::thread thread_;
    std::atomic<bool> quit_{false};
    std::atomic<bool> playing_{false};
    std::string path_;
    bool loop_ = true;
#endif
};
