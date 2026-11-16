#pragma once
#include "Transform.hpp"
#include "ecs/Registry.hpp"
#include <string>
#include <vector>

namespace kow::scene {

class Scene {
public:
    explicit Scene(std::string name);
    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] kow::ecs::Registry& registry() { return registry_; }
    [[nodiscard]] const kow::ecs::Registry& registry() const { return registry_; }

private:
    std::string name_;
    kow::ecs::Registry registry_;
};

} // namespace kow::scene
