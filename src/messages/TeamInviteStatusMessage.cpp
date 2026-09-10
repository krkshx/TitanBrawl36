// TeamInviteStatusMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.

#include "titan/messages/TeamInviteStatusMessage.hpp"

namespace titan {

void TeamInviteStatusMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(statusCode_);
    stream().writeBoolean(entry_ != nullptr);
    if (entry_) entry_->encode(stream());
}

void TeamInviteStatusMessage::decode() {
    PiranhaMessage::decode();
    statusCode_ = stream().readInt();
    if (stream().readBoolean()) {
        entry_ = std::make_unique<TeamInvitationDataEntry>();
        entry_->decode(stream());
    }
}

} // namespace titan
