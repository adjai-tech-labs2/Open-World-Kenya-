#include "scene/scene.h"
#include "core/logging/logger.h"
#include "core/error_handling/result.h"

namespace kow {

Scene::Scene(const std::string& name) : name_(name) {
    KOW_LOG_DEBUG("scene", "Created scene: {}", name);
}

Scene::~Scene() {
    KOW_LOG_DEBUG("scene", "Destroyed scene: {}", name);
}

EntityHandle Scene::create_entity() {
    auto entity = manager_.create_entity();
    KOW_LOG_TRACE("scene", "Created entity in scene '{}': index={}, gen={}",
        name_, entity.index, entity.generation);
    return entity;
}

void Scene::destroy_entity(EntityHandle entity) {
    manager_.destroy_entity(entity);
    KOW_LOG_TRACE("scene", "Destroyed entity in scene '{}': index={}, gen={}",
        name_, entity.index, entity.generation);
}

void Scene::clear() {
    manager_.clear();
    KOW_LOG_INFO("scene", "Cleared scene: {}", name_);
}

size_t Scene::entity_count() const {
    return manager_.alive_count();
}

} // namespace kow