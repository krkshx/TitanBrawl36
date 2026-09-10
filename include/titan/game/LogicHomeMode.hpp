#pragma once

// LogicHomeMode — home game state root (minimal port).
// Only the surface needed by command execute() so far:
//   getPlayerAvatar @0x5543f0 = *(this+40),
//   getHome @0x598c2c = *(this+32).
// UI listener hooks (getGameListener @0x3a6658) are platform code and stay
// out; execute() implementations skip listener callbacks explicitly.

#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicClientHome.hpp"
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
    // getHome @0x598c2c.
    [[nodiscard]] LogicClientHome* getHome() const { return home_.get(); }
    void setHome(std::unique_ptr<LogicClientHome> home) {
        home_ = std::move(home);
    }

private:
    std::unique_ptr<LogicClientHome> home_; // +32
    std::unique_ptr<LogicClientAvatar> avatar_; // +40
};

} // namespace titan
