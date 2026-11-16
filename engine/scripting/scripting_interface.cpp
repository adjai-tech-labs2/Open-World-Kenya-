#include "scripting/scripting_interface.h"
#include "core/logging/logger.h"

namespace kow {

ScriptingManager& ScriptingManager::instance() {
    static ScriptingManager manager;
    return manager;
}

void ScriptingManager::initialize() {
    if (!engine_) {
        engine_ = std::make_unique<ScriptingEngineAdapter>();
    }
    engine_->initialize();
    KOW_LOG_INFO("scripting", "Scripting engine initialized: {}", engine_->engine_name());
}

void ScriptingManager::shutdown() {
    if (engine_) {
        engine_->shutdown();
        KOW_LOG_INFO("scripting", "Scripting engine shutdown");
    }
}

void ScriptingManager::update(float delta_time) {
    if (engine_) {
        engine_->update(delta_time);
    }
}

} // namespace kow