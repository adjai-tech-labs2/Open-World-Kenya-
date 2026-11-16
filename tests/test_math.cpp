#include "test_framework.hpp"
#include "math/Math.hpp"

using namespace kow::math;

TEST(math_vec3_operations) {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{4.0f, 5.0f, 6.0f};
    auto c = a + b;
    ASSERT_EQ(c.x, 5.0f);
    ASSERT_EQ(c.y, 7.0f);
    ASSERT_EQ(c.z, 9.0f);
    ASSERT_TRUE(near_equal(a.dot(b), 32.0f));
    return true;
}

TEST(math_vec3_normalize) {
    Vec3 v{3.0f, 4.0f, 0.0f};
    auto n = v.normalized();
    ASSERT_TRUE(near_equal(n.length(), 1.0f));
    ASSERT_TRUE(near_equal(n.x, 0.6f));
    ASSERT_TRUE(near_equal(n.y, 0.8f));
    return true;
}

TEST(math_quat_from_axis_angle) {
    auto q = Quat::from_axis_angle({0.0f, 0.0f, 1.0f}, kPi);
    ASSERT_TRUE(near_equal(q.w, 0.0f));
    ASSERT_TRUE(near_equal(q.z, 1.0f));
    return true;
}

TEST(math_mat4_translation) {
    auto m = Mat4::translation({1.0f, 2.0f, 3.0f});
    auto p = m.transform_point({0.0f, 0.0f, 0.0f});
    ASSERT_TRUE(near_equal(p.x, 1.0f));
    ASSERT_TRUE(near_equal(p.y, 2.0f));
    ASSERT_TRUE(near_equal(p.z, 3.0f));
    return true;
}

TEST(math_aabb_intersects) {
    AABB a{{0,0,0},{2,2,2}};
    AABB b{{1,1,1},{3,3,3}};
    ASSERT_TRUE(a.intersects(b));
    AABB c{{5,5,5},{7,7,7}};
    ASSERT_FALSE(a.intersects(c));
    return true;
}

TEST(math_clamp_lerp) {
    ASSERT_TRUE(near_equal(clamp(-5.0f, 0.0f, 10.0f), 0.0f));
    ASSERT_TRUE(near_equal(clamp(15.0f, 0.0f, 10.0f), 10.0f));
    ASSERT_TRUE(near_equal(clamp(5.0f, 0.0f, 10.0f), 5.0f));
    ASSERT_TRUE(near_equal(lerp(0.0f, 10.0f, 0.5f), 5.0f));
    return true;
}
