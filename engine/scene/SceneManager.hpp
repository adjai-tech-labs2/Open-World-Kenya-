#pragma once
#include "Scene.hpp"
#include "core/Result.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace kow::scene {

class SceneManager {
public:
    kow::core::Result<Scene*> create_scene(const std::string& name);
    [[nodiscard]] Scene* get_scene(const std::string& name);
    [[nodiscard]] const Scene* get_scene(const std::string& name) const;
    kow::core::Result<void> set_active(const std::string& name);
    [[nodiscard]] Scene* active_scene();
    [[nodiscard]] std::size_t scene_count() const { return scenes_.size(); }

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
    std::string active_name_;
};

} // namespace kow::scene
