#pragma once
#include "Config.hpp"
#include "FileSystem.hpp"
#include "Logger.hpp"
#include "ModuleRegistry.hpp"
#include "Result.hpp"
#include "Time.hpp"
#include "Version.hpp"
#include <memory>

namespace kow::core {

enum class AppState { Created, Initialized, Running, ShutDown, Failed };

class Application {
public:
    explicit Application(EngineConfig config = {});
    ~Application();

    Result<void> initialize();
    Result<void> run();
    Result<void> shutdown();

    void request_stop() { stop_requested_ = true; }
    [[nodiscard]] bool stop_requested() const { return stop_requested_; }

    [[nodiscard]] AppState state() const { return state_; }
    [[nodiscard]] const EngineConfig& config() const { return config_; }
    [[nodiscard]] Logger& logger() { return logger_; }
    [[nodiscard]] FrameTimer& timer() { return timer_; }
    [[nodiscard]] FileSystem& filesystem() { return *filesystem_; }
    [[nodiscard]] ModuleRegistry& modules() { return modules_; }

    void set_filesystem(std::unique_ptr<FileSystem> fs);

    // Hook for custom per-frame work in tests/sandbox
    virtual void on_tick() {}

private:
    EngineConfig config_;
    Logger logger_;
    FrameTimer timer_;
    std::unique_ptr<FileSystem> filesystem_;
    ModuleRegistry modules_;
    AppState state_{AppState::Created};
    bool stop_requested_{false};
    bool modules_initialized_{false};
};

} // namespace kow::core
