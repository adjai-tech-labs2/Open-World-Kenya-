#pragma once
#include "scene/Transform.hpp"
#include "ecs/Entity.hpp"
#include "core/Result.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace kow::scene {

struct SceneNode {
    kow::ecs::EntityId entity{};
    std::string name;
    Transform transform{};
    std::vector<std::uint32_t> children{}; // indices into nodes_
    std::int32_t parent{-1};
};

class SceneGraph {
public:
    explicit SceneGraph(std::string name);

    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] std::size_t node_count() const { return nodes_.size(); }

    kow::core::Result<std::uint32_t> add_node(std::string node_name, kow::ecs::EntityId entity, const Transform& transform, std::int32_t parent = -1);
    [[nodiscard]] SceneNode* node(std::size_t index);
    [[nodiscard]] const SceneNode* node(std::size_t index) const;
    [[nodiscard]] kow::ecs::EntityId root_entity() const { return root_entity_; }

private:
    std::string name_;
    std::vector<SceneNode> nodes_;
    kow::ecs::EntityId root_entity_{};
};

} // namespace kow::scene
