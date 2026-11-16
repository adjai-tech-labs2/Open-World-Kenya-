#include "Time.hpp"

namespace kow::core {

void FrameTimer::reset() {
    start_ = clock::now();
    last_ = start_;
    delta_seconds_ = 0.0;
    total_seconds_ = 0.0;
    frame_count_ = 0;
    initialized_ = true;
}

void FrameTimer::tick() {
    if (!initialized_) {
        reset();
        return;
    }
    auto now = clock::now();
    std::chrono::duration<double> delta = now - last_;
    std::chrono::duration<double> total = now - start_;
    delta_seconds_ = delta.count();
    total_seconds_ = total.count();
    last_ = now;
    ++frame_count_;
}

} // namespace kow::core
