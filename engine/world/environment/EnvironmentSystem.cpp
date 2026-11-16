#include "EnvironmentSystem.hpp"

namespace kow::world::environment {

kow::core::Result<void> EnvironmentSystem::set_weather(WeatherType weather) {
    state.weather = weather;
    return kow::core::Ok();
}

kow::core::Result<void> EnvironmentSystem::advance_time(float hours) {
    state.time_of_day_hours += hours;
    while (state.time_of_day_hours >= 24.0f) state.time_of_day_hours -= 24.0f;
    while (state.time_of_day_hours < 0.0f) state.time_of_day_hours += 24.0f;
    return kow::core::Ok();
}

} // namespace kow::world::environment
