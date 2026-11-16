#pragma once

#include <cmath>
#include <cstdint>

namespace kow {

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }
    float length() const { return std::sqrt(x*x + y*y); }
    float dot(const Vec2& o) const { return x*o.x + y*o.y; }
};

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    float length() const { return std::sqrt(x*x + y*y + z*z); }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vec3 cross(const Vec3& o) const {
        return {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x};
    }
    Vec3 normalized() const {
        float len = length();
        if (len == 0.0f) return {0,0,0};
        return {x/len, y/len, z/len};
    }
};

struct Vec4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    Vec4() = default;
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct Mat4 {
    float m[16];

    Mat4() {
        for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    static Mat4 identity() {
        Mat4 mat;
        return mat;
    }

    static Mat4 perspective(float fov, float aspect, float near, float far);
    static Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up);
    static Mat4 translation(const Vec3& pos);
    static Mat4 scale(const Vec3& s);
};

struct Quat {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    Quat() = default;
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    static Quat identity() { return {0, 0, 0, 1}; }
    static Quat from_euler(float pitch, float yaw, float roll);
    Vec3 to_euler() const;
};

} // namespace kow