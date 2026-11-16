#include "core/time/timer.h"
#include "core/logging/logger.h"
#include <thread>

namespace kow {

Timer& Timer::instance(ClockType type) {
    static std::unordered_map<ClockType, std::unique_ptr<Timer>> timers;
    auto it = timers.find(type);
    if (it == timers.end()) {
        auto timer = std::make_unique<Timer>(type);
        auto* ptr = timer.get();
        timers[type] = std::move(timer);
        return *ptr;
    }
    return *it->second;
}

Timer::Timer(ClockType type) : type_(type) {
    info_.fixed_delta_time = fixed_timestep_;
}

void Timer::start() {
    running_ = true;
    paused_ = false;
    auto now = std::chrono::steady_clock::now();
    start_time_ = now;
    last_frame_time_ = now;
    current_time_ = now;
}

void Timer::stop() {
    running_ = false;
}

void Timer::reset() {
    auto now = std::chrono::steady_clock::now();
    start_time_ = now;
    last_frame_time_ = now;
    current_time_ = now;
    info_.time_since_start = 0.0f;
    info_.frame_count = 0;
    fixed_accumulator_ = 0.0f;
}

void Timer::pause() {
    paused_ = true;
    info_.is_paused = true;
}

void Timer::resume() {
    if (!paused_) return;
    paused_ = false;
    info_.is_paused = false;
    auto now = std::chrono::steady_clock::now();
    last_frame_time_ = now;
}

void Timer::set_time_scale(float scale) {
    time_scale_ = std::clamp(scale, 0.0f, 10.0f);
    info_.time_scale = time_scale_;
}

float Timer::delta_time() const {
    return info_.delta_time;
}

float Timer::total_time() const {
    return info_.time_since_start;
}

uint64_t Timer::frame_count() const {
    return static_cast<uint64_t>(info_.frame_count);
}

float Timer::fps() const {
    return info_.fps;
}

float Timer::fps_smoothed() const {
    return info_.fps_smooth;
}

bool Timer::is_running() const {
    return running_;
}

bool Timer::is_paused() const {
    return paused_;
}

void Timer::set_fixed_timestep(float dt) {
    fixed_timestep_ = std::max(0.001f, dt);
    info_.fixed_delta_time = fixed_timestep_;
}

float Timer::fixed_timestep() const {
    return fixed_timestep_;
}

float Timer::accumulate_fixed_steps() {
    if (paused_) return 0.0f;
    
    float fixed_steps = 0.0f;
    while (fixed_accumulator_ >= fixed_timestep_ && fixed_steps < 10.0f) {
        fixed_accumulator_ -= fixed_timestep_;
        fixed_steps += fixed_timestep_;
    }
    return fixed_steps;
}

ScopedTimer::ScopedTimer(const std::string& name) 
    : name_(name), start_(std::chrono::steady_clock::now()) {}

ScopedTimer::~ScopedTimer() {
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();
    KOW_LOG_TRACE("timer", "{} took {} us", name_, duration);
}

} // namespace kow