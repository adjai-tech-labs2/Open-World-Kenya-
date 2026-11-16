#include "test_framework.hpp"
#include "world/traffic/TrafficSystem.hpp"

using namespace kow::world::traffic;

TEST(traffic_system_add_light) {
    TrafficSystem ts;
    SectorCoord c{0, 0};
    ASSERT_TRUE(ts.add_light(1, c, TrafficLightState::Red).ok());
    ASSERT_EQ(ts.light_count(), static_cast<std::size_t>(1));
    auto* l = ts.light(1);
    ASSERT_TRUE(l != nullptr);
    ASSERT_EQ(l->state, TrafficLightState::Red);
    return true;
}

TEST(traffic_system_duplicate_light) {
    TrafficSystem ts;
    SectorCoord c{0, 0};
    ASSERT_TRUE(ts.add_light(1, c).ok());
    ASSERT_FALSE(ts.add_light(1, c).ok());
    return true;
}

TEST(traffic_system_cycle) {
    TrafficSystem ts;
    SectorCoord c{0, 0};
    ts.add_light(1, c, TrafficLightState::Red);
    ts.update(6.0f); // Red -> Green
    auto* l = ts.light(1);
    ASSERT_TRUE(l != nullptr);
    ASSERT_EQ(l->state, TrafficLightState::Green);
    ts.update(6.0f); // Green -> Yellow
    ASSERT_EQ(l->state, TrafficLightState::Yellow);
    ts.update(3.0f); // Yellow -> Red
    ASSERT_EQ(l->state, TrafficLightState::Red);
    return true;
}
