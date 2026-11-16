#include "core/modules/module_registry.h"
#include "core/logging/logger.h"

namespace kow {

ModuleRegistry& ModuleRegistry::instance() {
    static ModuleRegistry registry;
    return registry;
}

VoidResult ModuleRegistry::initialize_all() {
    std::lock_guard lock(mutex_);
    for (auto& [type, module] : modules_) {
        auto result = module->initialize();
        if (!result) {
            KOW_LOG_ERROR("module", "Failed to initialize {}: {}",
                module->name(), result.error().message);
            return result;
        }
        KOW_LOG_INFO("module", "Initialized: {}", module->name());
    }
    return success();
}

void ModuleRegistry::shutdown_all() {
    std::lock_guard lock(mutex_);
    for (auto& [type, module] : modules_) {
        module->shutdown();
        KOW_LOG_INFO("module", "Shutdown: {}", module->name());
    }
}

void ModuleRegistry::update_all(float delta_time) {
    std::lock_guard lock(mutex_);
    for (auto& [type, module] : modules_) {
        module->update(delta_time);
    }
}

size_t ModuleRegistry::module_count() const {
    std::lock_guard lock(mutex_);
    return modules_.size();
}

} // namespace kow