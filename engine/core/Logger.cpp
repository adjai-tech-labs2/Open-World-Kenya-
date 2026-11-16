#include "Logger.hpp"
#include <iostream>

namespace kow::core {

class StdoutSink : public ILogSink {
public:
    void write(const LogEntry& entry) override {
        std::ostream& os = (entry.level >= LogLevel::Error) ? std::cerr : std::cout;
        os << "[" << to_string(entry.level) << "] " << entry.message << std::endl;
    }
};

Logger::Logger(LogLevel min_level) : min_level_(min_level) {
    sinks_.push_back(std::make_shared<StdoutSink>());
}

void Logger::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    min_level_ = level;
}

LogLevel Logger::level() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return min_level_;
}

void Logger::add_sink(std::shared_ptr<ILogSink> sink) {
    std::lock_guard<std::mutex> lock(mutex_);
    sinks_.push_back(std::move(sink));
}

void Logger::clear_sinks() {
    std::lock_guard<std::mutex> lock(mutex_);
    sinks_.clear();
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (static_cast<int>(level) < static_cast<int>(min_level_)) return;
    LogEntry entry{level, message};
    for (auto& sink : sinks_) {
        if (sink) sink->write(entry);
    }
}

void Logger::trace(const std::string& msg) { log(LogLevel::Trace, msg); }
void Logger::debug(const std::string& msg) { log(LogLevel::Debug, msg); }
void Logger::info(const std::string& msg) { log(LogLevel::Info, msg); }
void Logger::warn(const std::string& msg) { log(LogLevel::Warn, msg); }
void Logger::error(const std::string& msg) { log(LogLevel::Error, msg); }
void Logger::critical(const std::string& msg) { log(LogLevel::Critical, msg); }

} // namespace kow::core
