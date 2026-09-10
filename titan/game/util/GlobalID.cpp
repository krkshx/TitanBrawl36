#pragma once

// GlobalID — reversed from libg_decrypted.so (ARM64).
//
// globalId = classId * 1000000 + instanceId
// GlobalID::getClassID @0x6a6b84: id / 1000000
// GlobalID::getInstanceID @0x9188a0: id % 1000000
// LogicData::getGlobalID @0x8a7118: return *(this+32)

#include "titan/core/Types.cpp"

namespace titan {

struct GlobalID {
    static constexpr i32 kInstancesPerClass = 1000000;

    static i32 make(i32 classId, i32 instanceId) {
        return classId * kInstancesPerClass + instanceId;
    }
    static i32 getClassID(i32 globalId) { return globalId / kInstancesPerClass; }   // @0x6a6b84
    static i32 getInstanceID(i32 globalId) { return globalId % kInstancesPerClass; } // @0x9188a0
};

} // namespace titan
