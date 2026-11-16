#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace kow::world::environment {

enum class WeatherType { Clear, Cloudy, Rain, Storm };

struct EnvironmentState {
    WeatherType weather{WeatherType::Clear};
    float temperature_c{22.0f}; // Nairobi average
    float humidity{0.60f};
    float wind_speed_mps{2.0f};
    float time_of_day_hours{6.0f}; // 0-24
};

class EnvironmentSystem {
public:
    EnvironmentState state{};

    kow::core::Result<void> set_weather(WeatherType weather);
    kow::core::Result<void> advance_time(float hours);
    [[nodiscard]] bool is_raining() const { return state.weather == WeatherType::Rain || state.weather == WeatherType::Storm; }
    [[nodiscard]] bool is_daytime() const { return state.time_of_day_hours >= 6.0f && state.time_of_day_hours < 18.0f; }
};

} // namespace kow::world::environment
