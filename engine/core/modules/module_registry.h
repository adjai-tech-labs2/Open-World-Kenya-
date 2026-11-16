#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <functional>
#include <typeindex>
#include "core/error_handling/result.h"

namespace kow {

class Module {
public:
    virtual ~Module() = default;
    virtual std::string_view name() const = 0;
    virtual VoidResult initialize() { return success(); }
    virtual void shutdown() {}
    virtual void update(float delta_time) {}
};

class ModuleRegistry {
public:
    static ModuleRegistry& instance();

    template<typename T>
    Result<T*> register_module(std::unique_ptr<T> module) {
        static_assert(std::is_base_of_v<Module, T>, "T must derive from Module");
        
        std::lock_guard lock(mutex_);
        
        auto* ptr = module.get();
        modules_[typeid(T)] = std::move(module);
        
        KOW_LOG_INFO("module", "Registered module: {}", ptr->name());
        return ptr;
    }

    template<typename T>
    T* get_module() const {
        std::lock_guard lock(mutex_);
        auto it = modules_.find(typeid(T));
        if (it != modules_.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool has_module() const {
        std::lock_guard lock(mutex_);
        return modules_.find(typeid(T)) != modules_.end();
    }

    VoidResult initialize_all();
    void shutdown_all();
    void update_all(float delta_time);

    size_t module_count() const;

private:
    ModuleRegistry() = default;
    mutable std::mutex mutex_;
    std::unordered_map<std::type_index, std::unique_ptr<Module>> modules_;
};

#define KOW_REGISTER_MODULE(type) \
    static auto* _reg_##type = &kow::ModuleRegistry::instance().get_or_create<type>()

} // namespace kow