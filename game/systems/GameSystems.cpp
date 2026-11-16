#include "GameSystems.hpp"

namespace kow::game::systems {

kow::core::Result<void> GameSystems::update(float dt) {
    (void)dt;
    ++update_count_;
    return kow::core::Ok();
}

} // namespace kow::game::systems
