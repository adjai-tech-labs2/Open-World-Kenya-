#include "diagnostics/profiler.h"
#include "core/logging/logger.h"
#include <thread>

namespace kow {

Profiler& Profiler::instance() {
    static Profiler profiler;
    return profiler;
}

Profiler::Profiler() {
    samples_.reserve(1000);
    stats_.reserve(256);
}

void Profiler::begin_sample(const std::string& name, ProfileCategory category) {
    if (!enabled_) return;
    
    std::unique_lock lock(mutex_);
    ProfileSample sample;
    sample.name = name;
    sample.category = category;
    sample.timestamp = std::chrono::steady_clock::now();
    sample.thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    samples_.push_back(std::move(sample));
}

void Profiler::end_sample(const std::string& name) {
    if (!enabled_) return;
    
    auto now = std::chrono::steady_clock::now();
    std::unique_lock lock(mutex_);
    
    for (auto it = samples_.rbegin(); it != samples_.rend(); ++it) {
        if (it->name == name && it->duration.count() == 0) {
            it->duration = std::chrono::duration_cast<std::chrono::microseconds>(now - it->timestamp);
            break;
        }
    }
}

void Profiler::begin_frame() {
    frame_start_ = std::chrono::steady_clock::now();
}

void Profiler::end_frame() {
    auto now = std::chrono::steady_clock::now();
    frame_time = std::chrono::duration_cast<std::chrono::microseconds>(now - frame_start_);
    
    // Aggregate stats
    std::unique_lock lock(mutex_);
    stats_.clear();
    
    std::unordered_map<std::string, std::vector<std::chrono::microseconds>> grouped;
    for (const auto& sample : samples_) {
        if (sample.duration.count() > 0) {
            grouped[sample.name].push_back(sample.duration);
        }
    }
    
    for (auto& [name, durations] : grouped) {
        ProfileStats stats;
        stats.name = name;
        stats.call_count = durations.size();
        
        auto total = std::chrono::microseconds{0};
        auto min = std::chrono::microseconds::max();
        auto max = std::chrono::microseconds{0};
        
        for (const auto& d : durations) {
            total += d;
            if (d < min) min = d;
            if (d > max) max = d;
        }
        
        stats.total_time = total;
        stats.min_time = min;
        stats.max_time = max;
        stats.avg_time_ms = std::chrono::duration<double, std::milli>(total).count() / durations.size();
        stats.fps_contribution = std::chrono::duration<double, std::milli>(total).count() / 16.667;
        
        stats_[name] = stats;
    }
    
    // Trim samples
    if (samples_.size() > max_samples_) {
        samples_.erase(samples_.begin(), samples_.begin() + (samples_.size() - max_samples_));
    }
}

std::vector<ProfileStats> Profiler::get_stats() const {
    std::shared_lock lock(mutex_);
    std::vector<ProfileStats> result;
    for (const auto& [name, stats] : stats_) {
        result.push_back(stats);
    }
    std::sort(result.begin(), result.end(), 
        [](const ProfileStats& a, const ProfileStats& b) {
            return a.total_time > b.total_time;
        });
    return result;
}

ProfileStats Profiler::get_stats(const std::string& name) const {
    std::shared_lock lock(mutex_);
    auto it = stats_.find(name);
    if (it != stats_.end()) return it->second;
    return ProfileStats{};
}

void Profiler::reset_stats() {
    std::unique_lock lock(mutex_);
    stats_.clear();
    samples_.clear();
}

size_t Profiler::sample_count() const {
    std::shared_lock lock(mutex_);
    return samples_.size();
}

ScopedProfile::ScopedProfile(const std::string& name, ProfileCategory category)
    : name_(name), category_(category) {
    Profiler::instance().begin_sample(name, category);
}

ScopedProfile::~ScopedProfile() {
    Profiler::instance().end_sample(name_);
}

} // namespace kow