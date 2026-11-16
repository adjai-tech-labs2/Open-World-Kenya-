#pragma once

#include <cstdint>
#include <string>
#include <source_location>
#include <chrono>

namespace kow {

enum class LogSeverity : uint8_t {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Fatal = 5
};

inline const char* severity_to_string(LogSeverity severity) {
    switch (severity) {
        case LogSeverity::Trace: return "TRACE";
        case LogSeverity::Debug: return "DEBUG";
        case LogSeverity::Info:  return "INFO";
        case LogSeverity::Warn:  return "WARN";
        case LogSeverity::Error: return "ERROR";
        case LogSeverity::Fatal: return "FATAL";
        default: return "UNKNOWN";
    }
}

struct LogMessage {
    LogSeverity severity;
    std::string message;
    std::string category;
    std::source_location location;
    std::chrono::steady_clock::time_point timestamp;
};

using LogSink = std::function<void(const LogMessage&)>;

class Logger {
public:
    static Logger& instance();

    void set_min_severity(LogSeverity severity) { min_severity_ = severity; }
    LogSeverity min_severity() const { return min_severity_; }

    void add_sink(LogSink sink);
    void remove_sinks();

    void log(LogSeverity severity, const std::string& category,
             const std::string& message, const std::source_location& loc);

    template<typename... Args>
    void log_format(LogSeverity severity, const std::string& category,
                    const std::string& format, Args&&... args, const std::source_location& loc = std::source_location::current());

private:
    Logger() = default;
    LogSeverity min_severity_ = LogSeverity::Info;
    std::vector<LogSink> sinks_;
};

#define KOW_LOG(severity, category, ...) \
    kow::Logger::instance().log_format(severity, category, __VA_ARGS__)

#define KOW_LOG_TRACE(category, ...) KOW_LOG(kow::LogSeverity::Trace, category, __VA_ARGS__)
#define KOW_LOG_DEBUG(category, ...) KOW_LOG(kow::LogSeverity::Debug, category, __VA_ARGS__)
#define KOW_LOG_INFO(category, ...)  KOW_LOG(kow::LogSeverity::Info, category, __VA_ARGS__)
#define KOW_LOG_WARN(category, ...)  KOW_LOG(kow::LogSeverity::Warn, category, __VA_ARGS__)
#define KOW_LOG_ERROR(category, ...) KOW_LOG(kow::LogSeverity::Error, category, __VA_ARGS__)
#define KOW_LOG_FATAL(category, ...) KOW_LOG(kow::LogSeverity::Fatal, category, __VA_ARGS__)

} // namespace kow