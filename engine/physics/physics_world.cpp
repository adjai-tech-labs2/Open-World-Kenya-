#include "physics/physics_world.h"
#include "core/logging/logger.h"

namespace kow {

PhysicsManager& PhysicsManager::instance() {
    static PhysicsManager manager;
    return manager;
}

void PhysicsManager::initialize() {
    if (!world_) {
        world_ = std::make_unique<PhysicsWorldAdapter>();
    }
    world_->initialize();
    KOW_LOG_INFO("physics", "Physics world initialized");
}

void PhysicsManager::shutdown() {
    if (world_) {
        world_->shutdown();
        KOW_LOG_INFO("physics", "Physics world shutdown");
    }
}

void PhysicsManager::update(float delta_time) {
    if (world_) {
        world_->update(delta_time);
    }
}

} // namespace kow