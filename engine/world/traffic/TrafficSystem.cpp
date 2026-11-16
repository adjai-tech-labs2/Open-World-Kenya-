#include "TrafficSystem.hpp"

namespace kow::world::traffic {

kow::core::Result<void> TrafficSystem::add_light(std::uint32_t id, SectorCoord coord, TrafficLightState initial) {
    if (lights_.find(id) != lights_.end()) {
        return kow::core::Err("traffic.light_exists", "Traffic light id already exists");
    }
    lights_.emplace(id, TrafficLight{id, coord, initial, 0.0f});
    return kow::core::Ok();
}

TrafficLight* TrafficSystem::light(std::uint32_t id) {
    auto it = lights_.find(id);
    return it == lights_.end() ? nullptr : &it->second;
}

const TrafficLight* TrafficSystem::light(std::uint32_t id) const {
    auto it = lights_.find(id);
    return it == lights_.end() ? nullptr : &it->second;
}

void TrafficSystem::update(float dt) {
    for (auto& [_, light] : lights_) {
        light.timer_seconds += dt;
        // Simple fixed-cycle: Red 5s, Yellow 2s, Green 5s
        if (light.state == TrafficLightState::Red && light.timer_seconds >= 5.0f) {
            light.state = TrafficLightState::Green;
            light.timer_seconds = 0.0f;
        } else if (light.state == TrafficLightState::Green && light.timer_seconds >= 5.0f) {
            light.state = TrafficLightState::Yellow;
            light.timer_seconds = 0.0f;
        } else if (light.state == TrafficLightState::Yellow && light.timer_seconds >= 2.0f) {
            light.state = TrafficLightState::Red;
            light.timer_seconds = 0.0f;
        }
    }
}

} // namespace kow::world::traffic
