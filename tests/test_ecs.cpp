#include "test_framework.hpp"
#include "ecs/Registry.hpp"
#include "scene/Transform.hpp"

using namespace kow;

TEST(entity_creation_and_alive) {
    ecs::Registry reg;
    auto e = reg.create();
    ASSERT_TRUE(e.valid());
    ASSERT_TRUE(reg.alive(e));
    ASSERT_EQ(reg.alive_count(), static_cast<std::size_t>(1));
    return true;
}

TEST(entity_destruction) {
    ecs::Registry reg;
    auto e = reg.create();
    reg.destroy(e);
    ASSERT_FALSE(reg.alive(e));
    ASSERT_EQ(reg.alive_count(), static_cast<std::size_t>(0));
    // destroying again is safe/no-op
    reg.destroy(e);
    ASSERT_EQ(reg.alive_count(), static_cast<std::size_t>(0));
    return true;
}

TEST(generation_safe_entity_ids) {
    ecs::Registry reg;
    auto e1 = reg.create();
    auto old = e1;
    reg.destroy(e1);
    auto e2 = reg.create();
    ASSERT_TRUE(reg.alive(e2));
    ASSERT_FALSE(reg.alive(old)); // stale id must be invalid
    ASSERT_EQ(old.index, e2.index); // reused slot
    ASSERT_NE(old.generation, e2.generation);
    return true;
}

struct Health { int value{100}; };
struct Tag { int id{0}; };

TEST(component_storage_add_get_has_remove) {
    ecs::Registry reg;
    auto e = reg.create();
    ASSERT_FALSE(reg.has<Health>(e));
    reg.add<Health>(e, Health{75});
    ASSERT_TRUE(reg.has<Health>(e));
    auto* h = reg.get<Health>(e);
    ASSERT_TRUE(h != nullptr);
    ASSERT_EQ(h->value, 75);
    reg.add<Tag>(e, Tag{7});
    ASSERT_TRUE(reg.has<Tag>(e));
    reg.remove<Health>(e);
    ASSERT_FALSE(reg.has<Health>(e));
    ASSERT_TRUE(reg.has<Tag>(e));
    reg.destroy(e);
    ASSERT_FALSE(reg.has<Tag>(e)); // destroyed entity components inaccessible
    return true;
}
