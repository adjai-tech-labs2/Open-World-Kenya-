#include "ModuleRegistry.hpp"
#include "Application.hpp"

namespace kow::core {

void ModuleRegistry::add(std::unique_ptr<IModule> module) {
    modules_.push_back(std::move(module));
    initialized_.push_back(false);
}

Result<void> ModuleRegistry::initialize_all(Application& app, Logger& logger) {
    for (std::size_t i = 0; i < modules_.size(); ++i) {
        auto& m = modules_[i];
        logger.info("Initializing module: " + m->name());
        auto res = m->initialize(app);
        if (!res.ok()) {
            logger.error("Failed to initialize module '" + m->name() + "': " + res.error().message);
            // shutdown those initialized so far in reverse
            for (std::size_t j = i; j-- > 0;) {
                if (initialized_[j]) {
                    modules_[j]->shutdown(app);
                    initialized_[j] = false;
                }
            }
            return res;
        }
        initialized_[i] = true;
    }
    return Ok();
}

Result<void> ModuleRegistry::shutdown_all(Application& app, Logger& logger) {
    std::string errors;
    for (std::size_t i = modules_.size(); i-- > 0;) {
        if (!initialized_[i]) continue;
        logger.info("Shutting down module: " + modules_[i]->name());
        auto res = modules_[i]->shutdown(app);
        initialized_[i] = false;
        if (!res.ok()) {
            if (!errors.empty()) errors += "; ";
            errors += modules_[i]->name() + ": " + res.error().message;
            logger.error("Shutdown error in module '" + modules_[i]->name() + "': " + res.error().message);
        }
    }
    if (!errors.empty()) {
        return Err("module.shutdown_failed", errors);
    }
    return Ok();
}

} // namespace kow::core
