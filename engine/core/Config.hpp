#pragma once
#include "Result.hpp"
#include <string>

namespace kow::core {

enum class LogLevel {
    Trace = 0,
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

struct EngineConfig {
    std::string app_name{"KOW Sandbox"};
    LogLevel log_level{LogLevel::Info};
    double target_fps{60.0};
    bool headless{true};
    int max_frames{1}; // bootstrap: run limited frames/ticks for deterministic tests
};

inline std::string to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Trace: return "TRACE";
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warn: return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

Result<void> validate_config(const EngineConfig& config);

} // namespace kow::core
