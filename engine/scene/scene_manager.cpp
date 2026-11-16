#include "scene/scene_manager.h"
#include "core/logging/logger.h"

namespace kow {

void SceneStack::push(std::unique_ptr<Scene> scene) {
    if (!scene) return;
    stack_.push_back(std::move(scene));
    KOW_LOG_INFO("scene", "Pushed scene: {}", stack_.back()->name());
}

std::unique_ptr<Scene> SceneStack::pop() {
    if (stack_.empty()) return nullptr;
    auto scene = std::move(stack_.back());
    stack_.pop_back();
    KOW_LOG_INFO("scene", "Popped scene: {}", scene->name());
    return scene;
}

void SceneStack::pop_all() {
    while (!stack_.empty()) {
        pop();
    }
}

void SceneStack::clear() {
    stack_.clear();
}

void SceneStack::update(float delta_time) {
    for (auto& scene : stack_) {
        if (scene->is_active()) {
            // Scene update logic
        }
    }
}

UserManager& UserManager::instance() {
    static UserManager manager;
    return manager;
}

void UserManager::push_scene(const std::string& name) {
    auto it = factories_.find(name);
    if (it != factories_.end()) {
        auto scene = it->second->create_scene(name);
        stack_.push(std::move(scene));
    } else {
        KOW_LOG_ERROR("scene", "Scene factory not found: {}", name);
    }
}

void UserManager::pop_scene() {
    stack_.pop();
}

void UserManager::pop_all_scenes() {
    stack_.pop_all();
}

void UserManager::clear_scenes() {
    stack_.clear();
}

Scene* UserManager::current_scene() {
    return stack_.top();
}

const Scene* UserManager::current_scene() const {
    return stack_.top();
}

bool UserManager::has_scene(const std::string& name) const {
    return factories_.find(name) != factories_.end();
}

void UserManager::update(float delta_time) {
    stack_.update(delta_time);
}

void UserManager::register_factory(std::unique_ptr<ISceneFactory> factory) {
    factories_[std::string(factory->scene_name())] = std::move(factory);
}

} // namespace kow