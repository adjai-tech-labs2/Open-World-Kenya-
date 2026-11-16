#include "test_framework.hpp"
#include "world/environment/EnvironmentSystem.hpp"

using namespace kow::world::environment;

TEST(environment_system_weather) {
    EnvironmentSystem env;
    ASSERT_FALSE(env.is_raining());
    ASSERT_TRUE(env.is_daytime());
    ASSERT_TRUE(env.set_weather(WeatherType::Rain).ok());
    ASSERT_TRUE(env.is_raining());
    ASSERT_TRUE(env.set_weather(WeatherType::Clear).ok());
    ASSERT_FALSE(env.is_raining());
    return true;
}

TEST(environment_system_time) {
    EnvironmentSystem env;
    ASSERT_TRUE(env.advance_time(12.0f).ok());
    ASSERT_FALSE(env.is_daytime()); // 18:00
    ASSERT_TRUE(env.advance_time(12.0f).ok());
    ASSERT_TRUE(env.is_daytime()); // 6:00 next day
    return true;
}
