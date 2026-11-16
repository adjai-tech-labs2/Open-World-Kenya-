#pragma once
#include <chrono>

namespace kow::core {

class FrameTimer {
public:
    void reset();
    void tick();
    [[nodiscard]] double delta_seconds() const { return delta_seconds_; }
    [[nodiscard]] double total_seconds() const { return total_seconds_; }
    [[nodiscard]] std::uint64_t frame_count() const { return frame_count_; }

private:
    using clock = std::chrono::steady_clock;
    clock::time_point start_{};
    clock::time_point last_{};
    double delta_seconds_{0.0};
    double total_seconds_{0.0};
    std::uint64_t frame_count_{0};
    bool initialized_{false};
};

} // namespace kow::core
