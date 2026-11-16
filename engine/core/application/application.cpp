#include "core/application/application.h"
#include "core/modules/module_registry.h"
#include "core/time/timer.h"
#include "core/configuration/config.h"
#include "core/filesystem/filesystem.h"
#include "core/logging/logger.h"
#include "core/types/version.h"
#include <iostream>

namespace kow {

Engine::Engine() {
    version_ = get_engine_version();
}

Engine::~Engine() {
    shutdown();
}

Engine& Engine::instance() {
    static Engine engine;
    return engine;
}

bool Engine::initialize() {
    if (initialized_) {
        KOW_LOG_WARN("engine", "Engine already initialized");
        return true;
    }

    KOW_LOG_INFO("engine", "Initializing KOW Engine v{}", version_to_string(version_));

    setup_logging();
    setup_config();
    setup_filesystem();
    setup_platform();
    register_core_modules();

    auto& registry = ModuleRegistry::instance();
    auto result = registry.initialize_all();
    if (!result) {
        KOW_LOG_FATAL("engine", "Module initialization failed: {}", result.error().message);
        return false;
    }

    Timer::instance().start();
    initialized_ = true;
    running_ = true;

    KOW_LOG_INFO("engine", "Engine initialized successfully");
    return true;
}

void Engine::run() {
    if (!initialized_) {
        KOW_LOG_ERROR("engine", "Engine not initialized");
        return;
    }

    KOW_LOG_INFO("engine", "Starting main loop");

    while (running_) {
        auto frame_start = std::chrono::steady_clock::now();

        process_input();
        update_game();
        render_frame();

        auto frame_end = std::chrono::steady_clock::now();
        auto frame_duration = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
        float dt = frame_duration.count() / 1000000.0f;

        // Cap frame rate
        if (dt < 0.001666f) {
            std::this_thread::sleep_for(std::chrono::microseconds(
                static_cast<int>((0.001666f - dt) * 1000000.0f)));
        }
    }

    KOW_LOG_INFO("engine", "Main loop ended");
}

void Engine::shutdown() {
    if (!initialized_) return;

    KOW_LOG_INFO("engine", "Shutting down engine");

    running_ = false;
    Timer::instance().stop();

    auto& registry = ModuleRegistry::instance();
    registry.shutdown_all();

    Config::instance().save_to_file("config/engine_config.json");

    initialized_ = false;
    KOW_LOG_INFO("engine", "Engine shutdown complete");
}

void Engine::setup_logging() {
    auto& logger = Logger::instance();
    logger.set_min_severity(LogSeverity::Info);
    logger.add_sink(console_sink);
    logger.add_sink(file_sink("logs/engine.log"));
}

void Engine::setup_config() {
    auto& config = Config::instance();
    config.register_default("engine.max_fps", 60, "Maximum frames per second");
    config.register_default("engine.window_width", 1280, "Window width");
    config.register_default("engine.window_height", 720, "Window height");
    config.register_default("engine.fullscreen", false, "Fullscreen mode");
    config.register_default("engine.enable_vulkan", false, "Enable Vulkan renderer");
    config.register_default("engine.enable_physics", false, "Enable physics");
    config.register_default("engine.enable_audio", false, "Enable audio");
    config.register_default("engine.enable_scripting", false, "Enable scripting");
    config.register_default("engine.data_path", "data", "Data directory path");
    config.register_default("engine.log_path", "logs", "Log directory path");

    // Try to load config file
    config.load_from_file("config/engine_config.json");
}

void Engine::setup_filesystem() {
    Filesystem::instance().set_root(std::filesystem::current_path());
}

void Engine::register_core_modules() {
    // Core modules will be registered here
    KOW_LOG_INFO("engine", "Core modules registered");
}

void Engine::process_input() {
    // Input processing
}

void Engine::update_game() {
    auto& timer = Timer::instance();
    timer.update(); // This would be called by platform layer
    
    auto& registry = ModuleRegistry::instance();
    registry.update_all(timer.delta_time());
}

void Engine::render_frame() {
    // Rendering
}

} // namespace kow