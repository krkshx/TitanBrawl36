#pragma once
#include "../core/PiranhaMessage.cpp"
#include <cstdint>
#include <string>
#include <vector>

class LoginFailedMessage : public PiranhaMessage {
public:
    std::int32_t getMessageType() const override { return 20103; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "LoginFailedMessage"; }

    void encode() override {}

    void decode() override {
        ByteStream *s = getByteStream();
        errorCode_ = s->readInt();
        str98_ = s->readString(900000);
        strA0_ = s->readString(900000);
        strA8_ = s->readString(900000);
        strB0_ = s->readString(900000);
        strB8_ = s->readString(900000);
        int88_ = s->readInt();
        boolC0_ = s->readBoolean();
        std::int32_t blobLen = s->readBytesLength();
        blob_ = s->readBytes(blobLen, 900000);
        std::int32_t n = s->readInt();
        urls_.clear();
        if (n >= 0 && n <= 1024 && !s->isAtEnd()) {
            for (std::int32_t i = 0; i < n; i++) {
                if (s->isAtEnd()) {
                    break;
                }
                urls_.push_back(s->readString(900000));
            }
        }
        if (s->isAtEnd()) {
            return;
        }
        int90_ = s->readInt();
        int8C_ = s->readInt();
        strC8_ = s->readString(900000);
        intF0_ = s->readInt();
        boolC1_ = s->readBoolean();
        boolE0_ = s->readBoolean();
        strF8_ = s->readStringReference(900000);
        vint108_ = s->readVInt();
        str110_ = s->readStringReference(900000);
        if (s->isAtEnd()) {
            return;
        }
        if (s->readBoolean()) {
            LogicLong v;
            s->readLong(&v);
            longHigh_ = v.high;
            longLow_ = v.low;
        }
    }

    std::int32_t errorCode() const { return errorCode_; }

private:
    std::int32_t errorCode_ = 0;
    std::string str98_;
    std::string strA0_;
    std::string strA8_;
    std::string strB0_;
    std::string strB8_;
    std::int32_t int88_ = 0;
    bool boolC0_ = false;
    std::vector<std::uint8_t> blob_;
    std::vector<std::string> urls_;
    std::int32_t int90_ = 0;
    std::int32_t int8C_ = 0;
    std::string strC8_;
    std::int32_t intF0_ = 0;
    bool boolC1_ = false;
    bool boolE0_ = false;
    std::string strF8_;
    std::int32_t vint108_ = 0;
    std::string str110_;
    std::int32_t longHigh_ = 0;
    std::int32_t longLow_ = 0;
};
