#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <string>
#include <vector>

class LoginOkMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 20104; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "LoginOkMessage"; }

    void encode() override {}

    void decode() override {
        // Порядок строго по либе (_ZN14LoginOkMessage6decodeEv @ 0x8a61e0):
        // 2x LogicLong, 3x String, 3x Int, String, 3x Int, String(184),
        // дальше всё под isAtEnd-гардами.
        ByteStream *s = getByteStream();
        LogicLong id;
        s->readLong(&id);
        accountId_ = (static_cast<std::int64_t>(id.high) << 32) | static_cast<std::uint32_t>(id.low);
        LogicLong id2;
        s->readLong(&id2);
        accountId2_ = (static_cast<std::int64_t>(id2.high) << 32) | static_cast<std::uint32_t>(id2.low);
        passToken_ = s->readString(900000);
        strA0_ = s->readString(900000);
        strA8_ = s->readString(900000);
        major_ = s->readInt();
        build_ = s->readInt();
        minor_ = s->readInt();
        serverEnv_ = s->readString(900000);
        intD8_ = s->readInt();
        intDC_ = s->readInt();
        intE0_ = s->readInt();
        strB8_ = s->readString(900000);
        if (!s->isAtEnd()) {
            strE8_ = s->readString(50);
            strF0_ = s->readString(50);
        }
        if (!s->isAtEnd()) {
            intF8_ = s->readInt();
        }
        if (!s->isAtEnd()) {
            strB0_ = s->readString(900000);
        }
        if (!s->isAtEnd()) {
            str100_ = s->readString(900000);
        }
        if (!s->isAtEnd()) {
            str108_ = s->readString(900000);
        }
        if (!s->isAtEnd()) {
            int138_ = s->readInt();
        }
        if (!s->isAtEnd()) {
            str110_ = s->readString(900000);
        }
        if (!s->isAtEnd()) {
            readStringArray(*s, array128_);
            readStringArray(*s, array130_);
        }
        if (s->isAtEnd()) {
            return;
        }
        vint14C_ = s->readVInt();
        if (s->isAtEnd()) {
            return;
        }
        // LogicCompressedString::decode @ 0x5ccf08: u32 len + bytes(len, 900000).
        {
            std::int32_t n = s->readBytesLength();
            compressed_ = s->readBytes(n, 900000);
        }
        bool328_ = s->readBoolean();
        bool329inv_ = !s->readBoolean();
        if (s->isAtEnd()) {
            return;
        }
        str118_ = s->readString(900000);
        if (s->isAtEnd()) {
            return;
        }
        str150_ = s->readString(900000);
        if (s->isAtEnd()) {
            return;
        }
        str120_ = s->readString(900000);
        if (s->isAtEnd()) {
            return;
        }
        str158_ = s->readString(900000);
        if (s->isAtEnd()) {
            return;
        }
        str160_ = s->readString(900000);
        if (s->isAtEnd()) {
            return;
        }
        bool168_ = s->readBoolean();
    }

    std::int64_t accountId() const { return accountId_; }
    const std::string &passToken() const { return passToken_; }

private:
    static void readStringArray(ByteStream &s, std::vector<std::string> &out) {
        out.clear();
        std::int32_t n = s.readInt();
        if (n < 0 || n >= 100) {
            return;
        }
        for (std::int32_t i = 0; i < n; i++) {
            if (s.isAtEnd()) {
                break;
            }
            out.push_back(s.readString(900000));
        }
    }

    std::int64_t accountId_ = 0;
    std::int64_t accountId2_ = 0;
    std::string passToken_;
    std::string strA0_;
    std::string strA8_;
    std::int32_t major_ = 0;
    std::int32_t build_ = 0;
    std::int32_t minor_ = 0;
    std::string serverEnv_;
    std::int32_t intD8_ = 0;
    std::int32_t intDC_ = 0;
    std::int32_t intE0_ = 0;
    std::string strB8_;
    std::string strE8_;
    std::string strF0_;
    std::int32_t intF8_ = 0;
    std::string strB0_;
    std::string str100_;
    std::string str108_;
    std::int32_t int138_ = 0;
    std::string str110_;
    std::vector<std::string> array128_;
    std::vector<std::string> array130_;
    std::int32_t vint14C_ = 0;
    std::vector<std::uint8_t> compressed_;
    bool bool328_ = false;
    bool bool329inv_ = false;
    std::string str118_;
    std::string str150_;
    std::string str120_;
    std::string str158_;
    std::string str160_;
    bool bool168_ = false;
};
