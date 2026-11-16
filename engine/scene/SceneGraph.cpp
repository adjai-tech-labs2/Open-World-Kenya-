#include "SceneGraph.hpp"
#include "core/Result.hpp"

namespace kow::scene {

SceneGraph::SceneGraph(std::string name) : name_(std::move(name)) {}

kow::core::Result<std::uint32_t> SceneGraph::add_node(std::string node_name, kow::ecs::EntityId entity, const Transform& transform, std::int32_t parent) {
    if (node_name.empty()) {
        return kow::core::Err("scenegraph.invalid_name", "Scene node name must not be empty");
    }
    std::uint32_t index = static_cast<std::uint32_t>(nodes_.size());
    nodes_.push_back(SceneNode{entity, std::move(node_name), transform, {}, parent});
    if (parent >= 0 && static_cast<std::size_t>(parent) < nodes_.size() - 1) {
        nodes_[static_cast<std::size_t>(parent)].children.push_back(index);
    } else if (parent == -1 && root_entity_.index == std::numeric_limits<std::uint32_t>::max()) {
        root_entity_ = entity;
    }
    return kow::core::Ok(index);
}

SceneNode* SceneGraph::node(std::size_t index) {
    if (index >= nodes_.size()) return nullptr;
    return &nodes_[index];
}

const SceneNode* SceneGraph::node(std::size_t index) const {
    if (index >= nodes_.size()) return nullptr;
    return &nodes_[index];
}

} // namespace kow::scene
