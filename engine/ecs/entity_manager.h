#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <atomic>
#include <concepts>
#include "core/error_handling/result.h"

namespace kow {

struct EntityHandle {
    uint32_t index : 24;
    uint32_t generation : 8;

    bool is_valid() const { return generation > 0; }
    bool operator==(const EntityHandle& other) const {
        return index == other.index && generation == other.generation;
    }
    bool operator!=(const EntityHandle& other) const { return !(*this == other); }
};

constexpr EntityHandle NULL_ENTITY{0, 0};

class EntityManager {
public:
    static EntityManager& instance();

    EntityHandle create_entity();
    void destroy_entity(EntityHandle entity);
    bool is_alive(EntityHandle entity) const;
    void clear();

    template<typename T>
    void add_component(EntityHandle entity, T component);

    template<typename T>
    T* get_component(EntityHandle entity);

    template<typename T>
    bool has_component(EntityHandle entity) const;

    template<typename T>
    void remove_component(EntityHandle entity);

    template<typename T>
    std::vector<EntityHandle> get_entities_with_component();

    size_t entity_count() const { return entities_.size(); }
    size_t alive_count() const;

private:
    EntityManager();
    
    struct EntitySlot {
        EntityHandle handle;
        bool alive = false;
    };

    std::vector<EntitySlot> entities_;
    std::vector<uint32_t> free_list_;
    std::atomic<uint32_t> next_generation_{1};

    struct ComponentStorageBase {
        virtual ~ComponentStorageBase() = default;
        virtual void remove(uint32_t index) = 0;
        virtual bool has(uint32_t index) const = 0;
        virtual void clear() = 0;
    };

    template<typename T>
    struct ComponentStorage : ComponentStorageBase {
        std::vector<T> components;
        std::vector<bool> has_component;

        void add(uint32_t index, T component) {
            if (index >= components.size()) {
                components.resize(index + 1);
                has_component.resize(index + 1, false);
            }
            components[index] = std::move(component);
            has_component[index] = true;
        }

        void remove(uint32_t index) override {
            if (index < has_component.size()) {
                has_component[index] = false;
            }
        }

        bool has(uint32_t index) const override {
            return index < has_component.size() && has_component[index];
        }

        void clear() override {
            components.clear();
            has_component.clear();
        }

        T* get(uint32_t index) {
            if (index >= components.size() || !has_component[index]) return nullptr;
            return &components[index];
        }
    };

    std::unordered_map<std::type_index, std::unique_ptr<ComponentStorageBase>> component_storages_;

    template<typename T>
    ComponentStorage<T>* get_storage();
};

#define KOW_ENTITY_MANAGER kow::EntityManager::instance()

} // namespace kow