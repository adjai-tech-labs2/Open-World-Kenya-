#include "core/lifecycle/engine_lifecycle.h"
#include "core/application/application.h"
#include "core/logging/logger.h"

namespace kow {

void EngineLifecycle::initialize() {
    KOW_LOG_INFO("lifecycle", "Engine lifecycle: initialize");
    auto& engine = Engine::instance();
    if (!engine.initialize()) {
        KOW_LOG_FATAL("lifecycle", "Engine initialization failed");
        std::exit(1);
    }
}

void EngineLifecycle::run() {
    KOW_LOG_INFO("lifecycle", "Engine lifecycle: run");
    Engine::instance().run();
}

void EngineLifecycle::shutdown() {
    KOW_LOG_INFO("lifecycle", "Engine lifecycle: shutdown");
    Engine::instance().shutdown();
}

} // namespace kow