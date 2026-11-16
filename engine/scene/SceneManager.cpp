#include "SceneManager.hpp"

namespace kow::scene {

kow::core::Result<Scene*> SceneManager::create_scene(const std::string& name) {
    if (name.empty()) {
        return kow::core::Err("scene.invalid_name", "Scene name must not be empty");
    }
    if (scenes_.find(name) != scenes_.end()) {
        return kow::core::Err("scene.already_exists", "Scene already exists: " + name);
    }
    auto scene = std::make_unique<Scene>(name);
    Scene* ptr = scene.get();
    scenes_[name] = std::move(scene);
    if (active_name_.empty()) active_name_ = name;
    return kow::core::Ok(ptr);
}

Scene* SceneManager::get_scene(const std::string& name) {
    auto it = scenes_.find(name);
    return it == scenes_.end() ? nullptr : it->second.get();
}

const Scene* SceneManager::get_scene(const std::string& name) const {
    auto it = scenes_.find(name);
    return it == scenes_.end() ? nullptr : it->second.get();
}

kow::core::Result<void> SceneManager::set_active(const std::string& name) {
    if (scenes_.find(name) == scenes_.end()) {
        return kow::core::Err("scene.not_found", "Scene not found: " + name);
    }
    active_name_ = name;
    return kow::core::Ok();
}

Scene* SceneManager::active_scene() {
    if (active_name_.empty()) return nullptr;
    return get_scene(active_name_);
}

} // namespace kow::scene
