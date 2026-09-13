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
        ByteStream *s = getByteStream();
        LogicLong id;
        s->readLong(&id);
        accountId_ = (static_cast<std::int64_t>(id.high) << 32) | static_cast<std::uint32_t>(id.low);
        LogicLong id2;
        s->readLong(&id2);
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
        strE8_ = s->readString(900000);
        strF0_ = s->readString(900000);
        intF8_ = s->readInt();
        strB0_ = s->readString(900000);
        str100_ = s->readString(900000);
        str108_ = s->readString(900000);
        int138_ = s->readInt();
        str110_ = s->readString(900000);
        readStringArray(*s, array128_);
        readStringArray(*s, array130_);
        vint14C_ = s->readVInt();
    }

    std::int64_t accountId() const { return accountId_; }
    const std::string &passToken() const { return passToken_; }

private:
    static void readStringArray(ByteStream &s, std::vector<std::string> &out) {
        out.clear();
        std::int32_t n = s.readInt();
        if (n < 0 || n > 4096 || s.isAtEnd()) {
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
};
