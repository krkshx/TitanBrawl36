#pragma once
#include "ByteStream.cpp"

class ByteStreamHelper {
public:
    static void writeDataReference(ChecksumEncoder *s, std::int32_t classId, std::int32_t instanceId) {
        if (classId <= 0 || instanceId <= 0) {
            s->writeVInt(0);
            return;
        }
        s->writeVInt(classId);
        s->writeVInt(instanceId);
    }

    static void writeDataReference(ChecksumEncoder *s, std::int32_t globalId) {
        if (globalId <= 0) {
            s->writeVInt(0);
            return;
        }
        s->writeVInt(globalId / 1000000);
        s->writeVInt(globalId % 1000000);
    }

    static std::int32_t readDataReference(ByteStream *s) {
        std::int32_t classId = s->readVInt();
        if (classId <= 0) {
            return 0;
        }
        std::int32_t instanceId = s->readVInt();
        return classId * 1000000 + instanceId;
    }

    static std::int32_t readDataReference(ByteStream *s, std::int32_t *classId, std::int32_t *instanceId) {
        std::int32_t cls = s->readVInt();
        if (cls <= 0) {
            if (classId != nullptr) {
                *classId = 0;
            }
            if (instanceId != nullptr) {
                *instanceId = 0;
            }
            return 0;
        }
        std::int32_t inst = s->readVInt();
        if (classId != nullptr) {
            *classId = cls;
        }
        if (instanceId != nullptr) {
            *instanceId = inst;
        }
        return cls * 1000000 + inst;
    }

    static void writeLogicLong(ChecksumEncoder *s, const LogicLong *v) {
        s->writeInt(v->high);
        s->writeInt(v->low);
    }

    static void writeLogicLong(ChecksumEncoder *s, std::int32_t high, std::int32_t low) {
        s->writeInt(high);
        s->writeInt(low);
    }

    static LogicLong readLogicLong(ByteStream *s) {
        LogicLong v;
        v.high = s->readInt();
        v.low = s->readInt();
        return v;
    }

    static void readLogicLong(ByteStream *s, LogicLong *v) {
        v->high = s->readInt();
        v->low = s->readInt();
    }

    // Как в либе (_ZN16ByteStreamHelper15decodeLogicLongEP10ByteStreamP9LogicLong @ 0x6641c4):
    // пара VInt, используется в LogicClientAvatar::decode и декодах карт.
    static void decodeLogicLong(ByteStream *s, LogicLong *v) {
        v->high = s->readVInt();
        v->low = s->readVInt();
    }

    static void writeStringReference(ChecksumEncoder *s, const std::string &v) {
        s->writeStringReference(v);
    }

    static std::string readStringReference(ByteStream *s, std::int32_t maxLength) {
        return s->readStringReference(maxLength);
    }

    static void writeBytesWithoutLength(ChecksumEncoder *s, const char *v, std::int32_t n) {
        ByteStream *stream = static_cast<ByteStream *>(s);
        if (stream->isByteStream()) {
            stream->writeBytesWithoutLength(v, n);
        }
    }

    static std::vector<std::uint8_t> readBytes(ByteStream *s, std::int32_t maxLength) {
        std::int32_t length = s->readBytesLength();
        return s->readBytes(length, maxLength);
    }
};
