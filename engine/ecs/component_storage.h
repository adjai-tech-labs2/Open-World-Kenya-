#pragma once

#include <vector>
#include <typeindex>
#include <memory>
#include <concepts>

namespace kow {

class ComponentStorageBase {
public:
    virtual ~ComponentStorageBase() = default;
    virtual void remove(uint32_t index) = 0;
    virtual bool has(uint32_t index) const = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual void resize(size_t size) = 0;
};

template<typename T>
class ComponentStorage : public ComponentStorageBase {
public:
    std::vector<T> components;
    std::vector<bool> has_component;

    void add(uint32_t index, T component) {
        if (index >= components.size()) {
            resize(index + 1);
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

    size_t size() const override { return components.size(); }

    void resize(size_t size) override {
        components.resize(size);
        has_component.resize(size, false);
    }

    T* get(uint32_t index) {
        if (index >= components.size() || !has_component[index]) return nullptr;
        return &components[index];
    }
};

} // namespace kow