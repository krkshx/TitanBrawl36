#pragma once

// LogicHomeMode — home game state root (minimal port).
// Only the surface needed by command execute() so far:
//   getPlayerAvatar @0x5543f0 = *(this+40).
// UI listener hooks (getGameListener @0x3a6658) are platform code and stay
// out; execute() implementations skip listener callbacks explicitly.

#include "titan/game/LogicClientAvatar.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>

namespace titan {

class LogicHomeMode {
public:
    // getPlayerAvatar @0x5543f0.
    [[nodiscard]] LogicClientAvatar* getPlayerAvatar() const {
        return avatar_.get();
    }
    void setPlayerAvatar(std::unique_ptr<LogicClientAvatar> avatar) {
        avatar_ = std::move(avatar);
    }

private:
    std::unique_ptr<LogicClientAvatar> avatar_; // +40
};

} // namespace titan
