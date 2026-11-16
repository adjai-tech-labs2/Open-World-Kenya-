#include "test_framework.hpp"
#include "scene/SceneManager.hpp"
#include "scene/Transform.hpp"

using namespace kow;

TEST(scene_registration_and_retrieval) {
    scene::SceneManager mgr;
    ASSERT_EQ(mgr.scene_count(), static_cast<std::size_t>(0));
    auto r = mgr.create_scene("main");
    ASSERT_TRUE(r.ok());
    ASSERT_TRUE(r.value() != nullptr);
    ASSERT_EQ(mgr.scene_count(), static_cast<std::size_t>(1));
    ASSERT_TRUE(mgr.get_scene("main") != nullptr);
    ASSERT_TRUE(mgr.active_scene() != nullptr);
    ASSERT_EQ(mgr.active_scene()->name(), std::string("main"));
    return true;
}

TEST(scene_duplicate_and_invalid_name) {
    scene::SceneManager mgr;
    auto bad = mgr.create_scene("");
    ASSERT_FALSE(bad.ok());
    auto ok = mgr.create_scene("level1");
    ASSERT_TRUE(ok.ok());
    auto dup = mgr.create_scene("level1");
    ASSERT_FALSE(dup.ok());
    ASSERT_EQ(dup.error().code, std::string("scene.already_exists"));
    auto nf = mgr.set_active("missing");
    ASSERT_FALSE(nf.ok());
    return true;
}

TEST(scene_transform_component_on_entity) {
    scene::SceneManager mgr;
    auto r = mgr.create_scene("sim");
    ASSERT_TRUE(r.ok());
    auto* sc = r.value();
    auto e = sc->registry().create();
    sc->registry().add<scene::Transform>(e);
    ASSERT_TRUE(sc->registry().has<scene::Transform>(e));
    auto* t = sc->registry().get<scene::Transform>(e);
    ASSERT_TRUE(t != nullptr);
    ASSERT_TRUE(t->scale.x == 1.0f && t->scale.y == 1.0f && t->scale.z == 1.0f);
    return true;
}
