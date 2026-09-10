#pragma once

#include "titan/core/ByteStream.cpp"
#include <optional>

// DataReference — (classId, instanceId) pair pointing at LogicData.
// ByteStreamHelper::writeDataReference @0x3a5dec:
//   null -> writeVInt(0); else writeVInt(classId), writeVInt(instanceId).

namespace titan {

struct DataReference {
    i32 classId = 0;
    i32 instanceId = 0;

    void encode(ByteStream& s) const {
        s.writeVInt(classId);
        s.writeVInt(instanceId);
    }
    // Nullable form used by message fields (null -> single VInt 0).
    static void encodeNullable(ByteStream& s, const std::optional<DataReference>& ref) {
        if (!ref.has_value()) {
            s.writeVInt(0);
            return;
        }
        ref->encode(s);
    }
    static std::optional<DataReference> decodeNullable(ByteStream& s) {
        const i32 cls = s.readVInt();
        if (cls == 0) {
            return std::nullopt;
        }
        DataReference r;
        r.classId = cls;
        r.instanceId = s.readVInt();
        return r;
    }
    bool operator==(const DataReference& o) const {
        return classId == o.classId && instanceId == o.instanceId;
    }
};

} // namespace titan
