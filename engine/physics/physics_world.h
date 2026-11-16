#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "core/error_handling/result.h"
#include "scene/transform.h"

namespace kow {

struct Ray {
    Vec3 origin;
    Vec3 direction;
};

struct RaycastResult {
    bool hit = false;
    float distance = 0.0f;
    Vec3 point;
    Vec3 normal;
    void* user_data = nullptr;
};

struct ColliderShape {
    enum class Type : uint8_t {
        Sphere = 0,
        Box = 1,
        Capsule = 2,
        Mesh = 3,
        Plane = 4
    };

    Type type = Type::Sphere;
    Vec3 half_extents;
    float radius = 1.0f;
    float height = 1.0f;
};

struct RigidBody {
    Vec3 position;
    Vec3 velocity;
    Vec3 angular_velocity;
    float mass = 1.0f;
    float friction = 0.5f;
    float restitution = 0.3f;
    bool is_static = false;
    bool is_kinematic = false;
};

struct Constraint {
    enum class Type : uint8_t {
        Distance = 0,
        Hinge = 1,
        BallSocket = 2,
        Fixed = 3,
        Slider = 4
    };

    Type type = Type::Distance;
    void* body_a = nullptr;
    void* body_b = nullptr;
    float target_distance = 1.0f;
    float motor_torque = 0.0f;
    float motor_limit = 0.0f;
};

class IPhysicsWorld {
public:
    virtual ~IPhysicsWorld() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update(float delta_time) = 0;

    virtual void* add_rigid_body(const RigidBody& body) = 0;
    virtual void remove_rigid_body(void* body) = 0;
    virtual void set_rigid_body_position(void* body, const Vec3& position) = 0;
    virtual void set_rigid_body_velocity(void* body, const Vec3& velocity) = 0;
    virtual Vec3 get_rigid_body_position(void* body) const = 0;
    virtual Vec3 get_rigid_body_velocity(void* body) const = 0;

    virtual void* add_collider(void* body, const ColliderShape& shape) = 0;
    virtual void remove_collider(void* collider) = 0;

    virtual void* add_constraint(const Constraint& constraint) = 0;
    virtual void remove_constraint(void* constraint) = 0;

    virtual RaycastResult ray_cast(const Ray& ray, float max_distance = 1000.0f) = 0;
    virtual std::vector<RaycastResult> ray_cast_multiple(const Ray& ray, float max_distance = 1000.0f, uint32_t max_hits = 10) = 0;

    virtual void set_gravity(const Vec3& gravity) = 0;
    virtual Vec3 gravity() const = 0;

    virtual void set_simulation_enabled(bool enabled) = 0;
    virtual bool simulation_enabled() const = 0;

    virtual size_t rigid_body_count() const = 0;
    virtual size_t collider_count() const = 0;
};

class PhysicsWorldAdapter : public IPhysicsWorld {
public:
    using InitializeFn = std::function<bool()>;
    using UpdateFn = std::function<void(float)>;
    using AddBodyFn = std::function<void*(const RigidBody&)>;
    using RaycastFn = std::function<RaycastResult(const Ray&, float)>;

    void set_on_initialize(InitializeFn fn) { on_initialize_ = std::move(fn); }
    void set_on_update(UpdateFn fn) { on_update_ = std::move(fn); }
    void set_on_add_body(AddBodyFn fn) { on_add_body_ = std::move(fn); }
    void set_on_raycast(RaycastFn fn) { on_raycast_ = std::move(fn); }

    bool initialize() override {
        if (on_initialize_) return on_initialize_();
        return true;
    }

    void shutdown() override {}
    void update(float delta_time) override {
        if (on_update_) on_update_(delta_time);
    }

    void* add_rigid_body(const RigidBody& body) override {
        if (on_add_body_) return on_add_body_(body);
        return nullptr;
    }

    void remove_rigid_body(void* body) override {}
    void set_rigid_body_position(void* body, const Vec3& position) override {}
    void set_rigid_body_velocity(void* body, const Vec3& velocity) override {}
    Vec3 get_rigid_body_position(void* body) const override { return {}; }
    Vec3 get_rigid_body_velocity(void* body) const override { return {}; }

    void* add_collider(void* body, const ColliderShape& shape) override { return nullptr; }
    void remove_collider(void* collider) override {}

    void* add_constraint(const Constraint& constraint) override { return nullptr; }
    void remove_constraint(void* constraint) override {}

    RaycastResult ray_cast(const Ray& ray, float max_distance) override {
        if (on_raycast_) return on_raycast_(ray, max_distance);
        return RaycastResult{};
    }

    std::vector<RaycastResult> ray_cast_multiple(const Ray& ray, float max_distance, uint32_t max_hits) override {
        auto result = ray_cast(ray, max_distance);
        if (result.hit) return {result};
        return {};
    }

    void set_gravity(const Vec3& gravity) override { gravity_ = gravity; }
    Vec3 gravity() const override { return gravity_; }

    void set_simulation_enabled(bool enabled) override { simulation_enabled_ = enabled; }
    bool simulation_enabled() const override { return simulation_enabled_; }

    size_t rigid_body_count() const override { return 0; }
    size_t collider_count() const override { return 0; }

private:
    InitializeFn on_initialize_;
    UpdateFn on_update_;
    AddBodyFn on_add_body_;
    RaycastFn on_raycast_;
    Vec3 gravity_{0, -9.81f, 0};
    bool simulation_enabled_ = true;
};

class PhysicsManager {
public:
    static PhysicsManager& instance();

    void initialize();
    void shutdown();
    void update(float delta_time);

    IPhysicsWorld* world() { return world_.get(); }
    const IPhysicsWorld* world() const { return world_.get(); }

    void set_world(std::unique_ptr<IPhysicsWorld> world) { world_ = std::move(world); }

private:
    PhysicsManager() = default;
    std::unique_ptr<IPhysicsWorld> world_;
};

#define KOW_PHYSICS kow::PhysicsManager::instance().world()

} // namespace kow