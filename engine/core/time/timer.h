#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <memory>
#include "core/error_handling/result.h"

namespace kow {

enum class ClockType : uint8_t {
    RealTime,
    GameTime,
    AudioTime
};

struct TimeInfo {
    float delta_time = 0.0f;        // Time since last frame
    float fixed_delta_time = 0.016f; // Fixed timestep (60 FPS)
    float time_since_start = 0.0f;  // Total game time
    float frame_count = 0;          // Current frame number
    float fps = 0.0f;               // Current FPS
    float fps_smooth = 0.0f;        // Smoothed FPS
    bool is_paused = false;         // Game paused
    float time_scale = 1.0f;        // Time dilation
};

class Timer {
public:
    static Timer& instance(ClockType type = ClockType::GameTime);

    void start();
    void stop();
    void reset();
    void pause();
    void resume();
    void set_time_scale(float scale);
    
    float delta_time() const;
    float total_time() const;
    uint64_t frame_count() const;
    float fps() const;
    float fps_smoothed() const;
    bool is_running() const;
    bool is_paused() const;
    
    void set_fixed_timestep(float dt);
    float fixed_timestep() const;
    
    float accumulate_fixed_steps();
    
    ClockType type() const { return type_; }
    const TimeInfo& info() const { return info_; }

private:
    explicit Timer(ClockType type);
    
    ClockType type_;
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point last_frame_time_;
    std::chrono::steady_clock::time_point current_time_;
    bool running_ = false;
    bool paused_ = false;
    float time_scale_ = 1.0f;
    float fixed_timestep_ = 0.016f;
    float fixed_accumulator_ = 0.0f;
    TimeInfo info_;
    float fps_accumulator_ = 0.0f;
    int fps_frame_count_ = 0;
};

class ScopedTimer {
public:
    explicit ScopedTimer(const std::string& name);
    ~ScopedTimer();

private:
    std::string name_;
    std::chrono::steady_clock::time_point start_;
};

#define KOW_SCOPED_TIMER(name) kow::ScopedTimer _timer_##__LINE__(name)

} // namespace kow