#include "Application.hpp"

namespace kow::core {

Application::Application(EngineConfig config)
    : config_(std::move(config)),
      logger_(config_.log_level),
      filesystem_(std::make_unique<FileSystem>()) {}

Application::~Application() {
    if (state_ == AppState::Initialized || state_ == AppState::Running) {
        (void)shutdown();
    }
}

void Application::set_filesystem(std::unique_ptr<FileSystem> fs) {
    if (fs) filesystem_ = std::move(fs);
}

Result<void> Application::initialize() {
    if (state_ == AppState::Initialized || state_ == AppState::Running) {
        return Err("app.already_initialized", "Application already initialized");
    }
    auto cfg_res = validate_config(config_);
    if (!cfg_res.ok()) {
        state_ = AppState::Failed;
        return cfg_res;
    }
    logger_.set_level(config_.log_level);
    logger_.info("KOW Engine " + kEngineVersion.to_string() + " initializing: " + config_.app_name);
    timer_.reset();

    auto mod_res = modules_.initialize_all(*this, logger_);
    if (!mod_res.ok()) {
        state_ = AppState::Failed;
        return mod_res;
    }
    modules_initialized_ = true;
    state_ = AppState::Initialized;
    logger_.info("Application initialized successfully");
    return Ok();
}

Result<void> Application::run() {
    if (state_ != AppState::Initialized) {
        auto init_res = initialize();
        if (!init_res.ok()) return init_res;
    }
    state_ = AppState::Running;
    logger_.info("Application run started");
    int frames = 0;
    // max_frames == 0 means run until stop requested (not used by default bootstrap)
    while (!stop_requested_) {
        timer_.tick();
        on_tick();
        ++frames;
        if (config_.max_frames > 0 && frames >= config_.max_frames) {
            break;
        }
    }
    logger_.info("Application run finished after " + std::to_string(frames) + " frame(s)");
    return Ok();
}

Result<void> Application::shutdown() {
    if (state_ == AppState::ShutDown) {
        return Ok();
    }
    logger_.info("Application shutting down");
    Result<void> result = Ok();
    if (modules_initialized_) {
        result = modules_.shutdown_all(*this, logger_);
        modules_initialized_ = false;
    }
    state_ = AppState::ShutDown;
    if (result.ok()) {
        logger_.info("Application shutdown complete");
    }
    return result;
}

} // namespace kow::core
