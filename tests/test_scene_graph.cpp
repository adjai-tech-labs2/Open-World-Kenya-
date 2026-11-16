#include "test_framework.hpp"
#include "scene/SceneGraph.hpp"
#include "ecs/Registry.hpp"

using namespace kow::scene;
using namespace kow::ecs;

TEST(scene_graph_add_node) {
    Registry reg;
    SceneGraph graph("level1");
    auto e = reg.create();
    auto r = graph.add_node("root", e, Transform{{0,0,0},{0,0,0,1},{1,1,1}});
    ASSERT_TRUE(r.ok());
    ASSERT_EQ(graph.node_count(), static_cast<std::size_t>(1));
    ASSERT_TRUE(graph.root_entity() == e);
    return true;
}

TEST(scene_graph_child_node) {
    Registry reg;
    SceneGraph graph("level1");
    auto e1 = reg.create();
    auto e2 = reg.create();
    auto r1 = graph.add_node("parent", e1, Transform{});
    ASSERT_TRUE(r1.ok());
    auto r2 = graph.add_node("child", e2, Transform{}, static_cast<std::int32_t>(r1.value()));
    ASSERT_TRUE(r2.ok());
    ASSERT_EQ(graph.node_count(), static_cast<std::size_t>(2));
    auto* child = graph.node(1);
    ASSERT_TRUE(child != nullptr);
    ASSERT_EQ(child->parent, 0);
    return true;
}
