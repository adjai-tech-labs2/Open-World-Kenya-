#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace kow::diagnostics {

struct ProfileSample {
    std::string name;
    double milliseconds{0.0};
};

class Profiler {
public:
    class ScopedTimer {
    public:
        ScopedTimer(Profiler& profiler, std::string name);
        ~ScopedTimer();
    private:
        Profiler& profiler_;
        std::string name_;
        std::chrono::steady_clock::time_point start_;
    };

    void record(const std::string& name, double ms);
    [[nodiscard]] std::vector<ProfileSample> samples() const;
    void clear();
    [[nodiscard]] bool enabled() const { return enabled_; }
    void set_enabled(bool e) { enabled_ = e; }

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, double> totals_;
    bool enabled_{true};
};

} // namespace kow::diagnostics
