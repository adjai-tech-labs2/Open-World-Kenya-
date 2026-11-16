#pragma once
#include <cmath>
#include <cstdint>
#include <algorithm>

namespace kow::math {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kDegToRad = kPi / 180.0f;
constexpr float kRadToDeg = 180.0f / kPi;
constexpr float kEpsilon = 1e-6f;

inline float clamp(float v, float lo, float hi) { return std::max(lo, std::min(v, hi)); }
inline float lerp(float a, float b, float t) { return a + (b - a) * t; }
inline bool near_equal(float a, float b, float eps = kEpsilon) { return std::abs(a - b) <= eps; }

struct Vec2 {
    float x{0.0f}, y{0.0f};
    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}
    Vec2 operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s) const { return {x*s, y*s}; }
    float length_sq() const { return x*x + y*y; }
    float length() const { return std::sqrt(length_sq()); }
};

struct Vec3 {
    float x{0.0f}, y{0.0f}, z{0.0f};
    Vec3() = default;
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(float s) const { return {x*s, y*s, z*s}; }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vec3 cross(const Vec3& o) const {
        return {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x};
    }
    float length_sq() const { return dot(*this); }
    float length() const { return std::sqrt(length_sq()); }
    Vec3 normalized() const {
        float len = length();
        if (len <= kEpsilon) return {0,0,0};
        return {x/len, y/len, z/len};
    }
    bool operator==(const Vec3& o) const { return near_equal(x,o.x) && near_equal(y,o.y) && near_equal(z,o.z); }
};

struct Vec4 {
    float x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f};
    Vec4() = default;
    Vec4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

struct Quat {
    float x{0.0f}, y{0.0f}, z{0.0f}, w{1.0f};
    Quat() = default;
    Quat(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    static Quat identity() { return {0,0,0,1}; }
    Quat normalized() const {
        float len = std::sqrt(x*x + y*y + z*z + w*w);
        if (len <= kEpsilon) return identity();
        return {x/len, y/len, z/len, w/len};
    }
    static Quat from_axis_angle(const Vec3& axis, float radians) {
        Vec3 n = axis.normalized();
        float half = radians * 0.5f;
        float s = std::sin(half);
        return Quat{n.x*s, n.y*s, n.z*s, std::cos(half)}.normalized();
    }
};

struct Mat4 {
    // Column-major 4x4
    float m[16]{};
    static Mat4 identity() {
        Mat4 r{};
        r.m[0]=1; r.m[5]=1; r.m[10]=1; r.m[15]=1;
        return r;
    }
    static Mat4 translation(const Vec3& t) {
        Mat4 r = identity();
        r.m[12]=t.x; r.m[13]=t.y; r.m[14]=t.z;
        return r;
    }
    static Mat4 scale(const Vec3& s) {
        Mat4 r{};
        r.m[0]=s.x; r.m[5]=s.y; r.m[10]=s.z; r.m[15]=1;
        return r;
    }
    Mat4 operator*(const Mat4& b) const {
        Mat4 r{};
        for (int c=0;c<4;++c) for (int row=0;row<4;++row) {
            r.m[c*4+row] = m[0*4+row]*b.m[c*4+0] + m[1*4+row]*b.m[c*4+1] +
                           m[2*4+row]*b.m[c*4+2] + m[3*4+row]*b.m[c*4+3];
        }
        return r;
    }
    Vec3 transform_point(const Vec3& v) const {
        float x = m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12];
        float y = m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13];
        float z = m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14];
        float w = m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15];
        if (std::abs(w) > kEpsilon) { x/=w; y/=w; z/=w; }
        return {x,y,z};
    }
};

struct AABB {
    Vec3 min{0,0,0};
    Vec3 max{0,0,0};
    bool contains(const Vec3& p) const {
        return p.x>=min.x && p.x<=max.x && p.y>=min.y && p.y<=max.y && p.z>=min.z && p.z<=max.z;
    }
    bool intersects(const AABB& o) const {
        return (min.x<=o.max.x && max.x>=o.min.x) &&
               (min.y<=o.max.y && max.y>=o.min.y) &&
               (min.z<=o.max.z && max.z>=o.min.z);
    }
    Vec3 center() const { return {(min.x+max.x)*0.5f,(min.y+max.y)*0.5f,(min.z+max.z)*0.5f}; }
    Vec3 extents() const { return {(max.x-min.x)*0.5f,(max.y-min.y)*0.5f,(max.z-min.z)*0.5f}; }
};

struct TransformTRS {
    Vec3 position{0,0,0};
    Quat rotation = Quat::identity();
    Vec3 scale{1,1,1};
    Mat4 to_matrix() const {
        // Simplified: translation * scale (rotation matrix expansion omitted in stub math; documented limitation)
        return Mat4::translation(position) * Mat4::scale(scale);
    }
};

} // namespace kow::math
