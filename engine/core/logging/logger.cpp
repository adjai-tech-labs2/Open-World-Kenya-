#include "core/logging/logger.h"

#include <cstdio>
#include <vector>
#include <format>
#include <iostream>
#include <sink.h>

namespace kow {

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::add_sink(LogSink sink) {
    sinks_.push_back(std::move(sink));
}

void Logger::remove_sinks() {
    sinks_.clear();
}

void Logger::log(LogSeverity severity, const std::string& category,
                 const std::string& message, const std::source_location& loc) {
    if (severity < min_severity_) return;

    LogMessage log_msg;
    log_msg.severity = severity;
    log_msg.message = message;
    log_msg.category = category;
    log_msg.location = loc;
    log_msg.timestamp = std::chrono::steady_clock::now();

    for (const auto& sink : sinks_) {
        sink(log_msg);
    }
}

template<typename... Args>
void Logger::log_format(LogSeverity severity, const std::string& category,
                        const std::string& format, Args&&... args,
                        const std::source_location& loc) {
    try {
        std::string message = std::format(format, std::forward<Args>(args)...);
        log(severity, category, message, loc);
    } catch (const std::exception& e) {
        std::string error_msg = std::format("Format error: {} | Original: {}", e.what(), format);
        log(LogSeverity::Error, "logger", error_msg, loc);
    }
}

// Console sink
void console_sink(const LogMessage& msg) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &now_c);
    
    std::cout << std::format("[{:02d}:{:02d}:{:02d}] [{}] [{}] {}\n",
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        severity_to_string(msg.severity),
        msg.category,
        msg.message);
}

// File sink factory
LogSink file_sink(const std::string& filepath) {
    auto* file = std::fopen(filepath.c_str(), "a");
    if (!file) {
        return [](const LogMessage&) {};
    }
    
    return [file](const LogMessage& msg) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &now_c);
        
        std::fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] [%s] %s\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec,
            severity_to_string(msg.severity),
            msg.category.c_str(),
            msg.message.c_str());
        std::fflush(file);
    };
}

} // namespace kow