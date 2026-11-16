#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace kow::world::traffic {

enum class TrafficLightState { Red, Yellow, Green };

struct TrafficLight {
    std::uint32_t id{0};
    SectorCoord coord{};
    TrafficLightState state{TrafficLightState::Red};
    float timer_seconds{0.0f};
};

class TrafficSystem {
public:
    kow::core::Result<void> add_light(std::uint32_t id, SectorCoord coord, TrafficLightState initial = TrafficLightState::Red);
    [[nodiscard]] TrafficLight* light(std::uint32_t id);
    [[nodiscard]] const TrafficLight* light(std::uint32_t id) const;
    void update(float dt);
    [[nodiscard]] std::size_t light_count() const { return lights_.size(); }

private:
    std::unordered_map<std::uint32_t, TrafficLight> lights_;
};

} // namespace kow::world::traffic
