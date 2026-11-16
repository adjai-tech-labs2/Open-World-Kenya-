#pragma once

#include <string>
#include <memory>
#include "core/application/application.h"

namespace kow {

class EngineLifecycle {
public:
    static void initialize();
    static void run();
    static void shutdown();
    static bool is_initialized() { return Engine::instance().is_running(); }
};

} // namespace kow