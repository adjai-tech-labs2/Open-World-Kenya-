#include "test_framework.hpp"
#include "core/events/EventBus.hpp"

using namespace kow::core;

TEST(event_bus_subscribe_and_publish) {
    EventBus bus;
    int received = 0;
    bus.subscribe<FrameBeginEvent>([&](const FrameBeginEvent& e) { received = static_cast<int>(e.frame); });
    bus.publish(FrameBeginEvent{42, 0.016f});
    ASSERT_EQ(received, 42);
    return true;
}

TEST(event_bus_multiple_handlers) {
    EventBus bus;
    int count_a = 0, count_b = 0;
    bus.subscribe<EngineEvent>([&](const EngineEvent&) { ++count_a; });
    bus.subscribe<EngineEvent>([&](const EngineEvent&) { ++count_b; });
    bus.publish(EngineEvent{"test"});
    ASSERT_EQ(count_a, 1);
    ASSERT_EQ(count_b, 1);
    return true;
}

TEST(event_bus_handler_count) {
    EventBus bus;
    ASSERT_EQ(bus.handler_count(), static_cast<std::size_t>(0));
    bus.subscribe<FrameBeginEvent>([](const FrameBeginEvent&) {});
    ASSERT_EQ(bus.handler_count(), static_cast<std::size_t>(1));
    bus.clear();
    ASSERT_EQ(bus.handler_count(), static_cast<std::size_t>(0));
    return true;
}
