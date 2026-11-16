#pragma once
#include "Entity.hpp"
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <any>

namespace kow::ecs {

class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    virtual void remove(EntityId id) = 0;
    virtual bool has(EntityId id) const = 0;
};

template <typename T>
class ComponentStorage : public IComponentStorage {
public:
    template <typename... Args>
    T& add(EntityId id, Args&&... args) {
        data_[id.index] = T{std::forward<Args>(args)...};
        present_[id.index] = id.generation;
        return data_[id.index];
    }

    void remove(EntityId id) override {
        if (id.index < present_.size() && present_[id.index] == id.generation) {
            present_[id.index] = 0;
        }
    }

    bool has(EntityId id) const override {
        return id.index < present_.size() && present_[id.index] == id.generation && id.generation != 0;
    }

    T* get(EntityId id) {
        if (!has(id)) return nullptr;
        return &data_[id.index];
    }
    const T* get(EntityId id) const {
        if (!has(id)) return nullptr;
        return &data_[id.index];
    }

private:
    std::unordered_map<std::uint32_t, T> data_;
    std::vector<std::uint32_t> present_;
};

class Registry {
public:
    EntityId create();
    void destroy(EntityId id);
    [[nodiscard]] bool alive(EntityId id) const;

    template <typename T, typename... Args>
    T& add(EntityId id, Args&&... args) {
        auto& storage = ensure_storage<T>();
        return storage.add(id, std::forward<Args>(args)...);
    }

    template <typename T>
    void remove(EntityId id) {
        auto* storage = get_storage<T>();
        if (storage) storage->remove(id);
    }

    template <typename T>
    [[nodiscard]] bool has(EntityId id) const {
        auto* storage = get_storage<T>();
        return storage && storage->has(id);
    }

    template <typename T>
    T* get(EntityId id) {
        auto* storage = get_storage<T>();
        return storage ? storage->get(id) : nullptr;
    }

    template <typename T>
    const T* get(EntityId id) const {
        auto* storage = get_storage<T>();
        return storage ? storage->get(id) : nullptr;
    }

    [[nodiscard]] std::size_t alive_count() const { return alive_count_; }

private:
    template <typename T>
    ComponentStorage<T>& ensure_storage() {
        auto ti = std::type_index(typeid(T));
        auto it = storages_.find(ti);
        if (it == storages_.end()) {
            auto ptr = std::make_unique<ComponentStorage<T>>();
            auto& ref = *ptr;
            storages_[ti] = std::move(ptr);
            return ref;
        }
        return *static_cast<ComponentStorage<T>*>(it->second.get());
    }

    template <typename T>
    ComponentStorage<T>* get_storage() {
        auto ti = std::type_index(typeid(T));
        auto it = storages_.find(ti);
        if (it == storages_.end()) return nullptr;
        return static_cast<ComponentStorage<T>*>(it->second.get());
    }
    template <typename T>
    const ComponentStorage<T>* get_storage() const {
        auto ti = std::type_index(typeid(T));
        auto it = storages_.find(ti);
        if (it == storages_.end()) return nullptr;
        return static_cast<const ComponentStorage<T>*>(it->second.get());
    }

    std::vector<std::uint32_t> generations_; // 0 = never used/free slot marker logic
    std::vector<std::uint32_t> free_indices_;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> storages_;
    std::size_t alive_count_{0};
};

} // namespace kow::ecs
