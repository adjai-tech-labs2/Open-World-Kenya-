#include "ecs/entity_manager.h"
#include "core/logging/logger.h"
#include <unordered_map>

namespace kow {

EntityManager& EntityManager::instance() {
    static EntityManager manager;
    return manager;
}

EntityManager::EntityManager() {
    entities_.reserve(1024);
    free_list_.reserve(1024);
}

EntityHandle EntityManager::create_entity() {
    uint32_t index;
    
    if (!free_list_.empty()) {
        index = free_list_.back();
        free_list_.pop_back();
    } else {
        index = static_cast<uint32_t>(entities_.size());
        entities_.emplace_back();
    }
    
    EntityHandle handle{index, next_generation_.fetch_add(1)};
    if (handle.generation == 0) handle.generation = 1; // 0 means invalid
    
    entities_[index].handle = handle;
    entities_[index].alive = true;
    
    return handle;
}

void EntityManager::destroy_entity(EntityHandle entity) {
    if (!entity.is_valid() || entity.index >= entities_.size()) return;
    
    auto& slot = entities_[entity.index];
    if (!slot.alive || slot.handle != entity) return;
    
    slot.alive = false;
    free_list_.push_back(entity.index);
    
    // Remove all components
    for (auto& [type, storage] : component_storages_) {
        storage->remove(entity.index);
    }
}

bool EntityManager::is_alive(EntityHandle entity) const {
    if (!entity.is_valid() || entity.index >= entities_.size()) return false;
    const auto& slot = entities_[entity.index];
    return slot.alive && slot.handle == entity;
}

void EntityManager::clear() {
    for (auto& [type, storage] : component_storages_) {
        storage->clear();
    }
    
    entities_.clear();
    free_list_.clear();
    next_generation_.store(1);
}

size_t EntityManager::alive_count() const {
    size_t count = 0;
    for (const auto& slot : entities_) {
        if (slot.alive) ++count;
    }
    return count;
}

template<typename T>
void EntityManager::add_component(EntityHandle entity, T component) {
    if (!is_alive(entity)) return;
    auto* storage = get_storage<T>();
    storage->add(entity.index, std::move(component));
}

template<typename T>
T* EntityManager::get_component(EntityHandle entity) {
    if (!is_alive(entity)) return nullptr;
    auto* storage = get_storage<T>();
    return storage->get(entity.index);
}

template<typename T>
bool EntityManager::has_component(EntityHandle entity) const {
    if (!is_alive(entity)) return false;
    auto it = component_storages_.find(typeid(T));
    if (it == component_storages_.end()) return false;
    return it->second->has(entity.index);
}

template<typename T>
void EntityManager::remove_component(EntityHandle entity) {
    if (!is_alive(entity)) return;
    auto it = component_storages_.find(typeid(T));
    if (it != component_storages_.end()) {
        it->second->remove(entity.index);
    }
}

template<typename T>
std::vector<EntityHandle> EntityManager::get_entities_with_component() {
    std::vector<EntityHandle> result;
    auto* storage = get_storage<T>();
    
    for (size_t i = 0; i < entities_.size(); ++i) {
        if (entities_[i].alive && storage->has(static_cast<uint32_t>(i))) {
            result.push_back(entities_[i].handle);
        }
    }
    return result;
}

template<typename T>
EntityManager::ComponentStorage<T>* EntityManager::get_storage() {
    auto it = component_storages_.find(typeid(T));
    if (it == component_storages_.end()) {
        auto storage = std::make_unique<ComponentStorage<T>>();
        auto* ptr = storage.get();
        component_storages_[typeid(T)] = std::move(storage);
        return ptr;
    }
    return static_cast<ComponentStorage<T>*>(it->second.get());
}

// Explicit template instantiations
template void EntityManager::add_component<int>(EntityHandle, int);
template int* EntityManager::get_component<int>(EntityHandle);
template bool EntityManager::has_component<int>(EntityHandle) const;
template void EntityManager::remove_component<int>(EntityHandle);
template std::vector<EntityHandle> EntityManager::get_entities_with_component<int>();

} // namespace kow