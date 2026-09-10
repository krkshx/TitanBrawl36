// Message batch 12 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch12.hpp"
#include "titan/messages/MsgBatch11.hpp" // LogicTencentAntiAddictionInstruction

namespace titan {

// ---- 24223 TencentCheckCanPayResponseMessage ----
void TencentCheckCanPayResponseMessage::encode() {
    PiranhaMessage::encode();
    stream().writeStringReference(data_);
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(instructions_.size()));
    for (const auto& x : instructions_) x->encode(stream());
}
void TencentCheckCanPayResponseMessage::decode() {
    PiranhaMessage::decode();
    data_ = stream().readStringReference();
    const i32 n = stream().readVInt();
    instructions_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        auto x = std::make_unique<LogicTencentAntiAddictionInstruction>();
        x->decode(stream());
        instructions_.push_back(std::move(x));
    }
}

// ---- 12103 UpdatePlayerMapMessage ----
void UpdatePlayerMapMessage::encode() {
    PiranhaMessage::encode();
    mapId_.encode(stream());
    if (!mapData_) throw pending_reverse("UpdatePlayerMapMessage needs LogicCompressedString");
    mapData_->encode(stream());
}
void UpdatePlayerMapMessage::decode() {
    PiranhaMessage::decode();
    mapId_ = LogicLong::decode(stream());
    mapData_ = std::make_unique<LogicCompressedString>();
    mapData_->decode(stream());
}

} // namespace titan
