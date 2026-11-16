#pragma once
#include "Config.hpp"
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace kow::core {

struct LogEntry {
    LogLevel level;
    std::string message;
};

class ILogSink {
public:
    virtual ~ILogSink() = default;
    virtual void write(const LogEntry& entry) = 0;
};

class VectorSink : public ILogSink {
public:
    void write(const LogEntry& entry) override {
        std::lock_guard<std::mutex> lock(mutex_);
        entries_.push_back(entry);
    }
    [[nodiscard]] std::vector<LogEntry> entries() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_;
    }
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        entries_.clear();
    }
private:
    mutable std::mutex mutex_;
    std::vector<LogEntry> entries_;
};

class Logger {
public:
    explicit Logger(LogLevel min_level = LogLevel::Info);

    void set_level(LogLevel level);
    [[nodiscard]] LogLevel level() const;
    void add_sink(std::shared_ptr<ILogSink> sink);
    void clear_sinks();

    void log(LogLevel level, const std::string& message);
    void trace(const std::string& msg);
    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);
    void critical(const std::string& msg);

private:
    LogLevel min_level_;
    std::vector<std::shared_ptr<ILogSink>> sinks_;
    mutable std::mutex mutex_;
};

} // namespace kow::core
