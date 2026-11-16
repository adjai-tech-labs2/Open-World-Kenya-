#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <functional>
#include <atomic>
#include <shared_mutex>
#include "core/time/timer.h"

namespace kow {

enum class ProfileCategory : uint8_t {
    Render = 0,
    Physics = 1,
    Audio = 2,
    Script = 3,
    AI = 4,
    Network = 5,
    Asset = 6,
    Game = 7,
    Other = 8
};

struct ProfileSample {
    std::string name;
    ProfileCategory category;
    std::chrono::microseconds duration;
    std::chrono::steady_clock::time_point timestamp;
    uint32_t thread_id;
};

struct ProfileStats {
    std::string name;
    uint64_t call_count = 0;
    std::chrono::microseconds total_time{0};
    std::chrono::microseconds min_time{0};
    std::chrono::microseconds max_time{0};
    double avg_time_ms = 0.0;
    double fps_contribution = 0.0;
};

class Profiler {
public:
    static Profiler& instance();

    void begin_sample(const std::string& name, ProfileCategory category = ProfileCategory::Other);
    void end_sample(const std::string& name);

    void begin_frame();
    void end_frame();

    std::vector<ProfileStats> get_stats() const;
    ProfileStats get_stats(const std::string& name) const;
    void reset_stats();
    void set_enabled(bool enabled) { enabled_ = enabled; }
    bool is_enabled() const { return enabled_; }

    void set_max_samples(size_t max_samples) { max_samples_ = max_samples; }
    size_t max_samples() const { return max_samples_; }

    size_t sample_count() const;

private:
    Profiler();
    std::vector<ProfileSample> samples_;
    std::unordered_map<std::string, ProfileStats> stats_;
    std::chrono::steady_clock::time_point frame_start_;
    std::chrono::microseconds frame_time{0};
    bool enabled_ = true;
    size_t max_samples_ = 1000;
    mutable std::shared_mutex mutex_;
};

class ScopedProfile {
public:
    ScopedProfile(const std::string& name, ProfileCategory category = ProfileCategory::Other);
    ~ScopedProfile();

private:
    std::string name_;
    ProfileCategory category_;
};

#define KOW_PROFILE(name) kow::ScopedProfile _profile_##__LINE__(name)
#define KOW_PROFILE_CATEGORY(name, cat) kow::ScopedProfile _profile_##__LINE__(name, cat)

} // namespace kow