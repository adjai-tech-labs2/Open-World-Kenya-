#pragma once
#include "core/Result.hpp"
#include <string>

namespace kow::physics {

// Interface only: no physics simulation backend implemented or claimed.
struct PhysicsConfig {
    float gravity_y{-9.81f};
    float fixed_timestep{1.0f / 60.0f};
};

class IPhysicsWorld {
public:
    virtual ~IPhysicsWorld() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual kow::core::Result<void> initialize(const PhysicsConfig& config) = 0;
    virtual void step(float dt) = 0;
    virtual kow::core::Result<void> shutdown() = 0;
    [[nodiscard]] virtual bool is_initialized() const = 0;
};

class NullPhysicsWorld final : public IPhysicsWorld {
public:
    [[nodiscard]] std::string name() const override { return "NullPhysicsWorld (no simulation backend)"; }
    kow::core::Result<void> initialize(const PhysicsConfig&) override { initialized_ = true; return kow::core::Ok(); }
    void step(float) override { /* no-op: no backend */ }
    kow::core::Result<void> shutdown() override { initialized_ = false; return kow::core::Ok(); }
    [[nodiscard]] bool is_initialized() const override { return initialized_; }
private:
    bool initialized_{false};
};

} // namespace kow::physics
