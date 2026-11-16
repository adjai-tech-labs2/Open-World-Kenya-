#include "scene/transform.h"
#include <cmath>

namespace kow {

Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
    Mat4 mat;
    float f = 1.0f / std::tan(fov * 0.5f);
    mat.m[0] = f / aspect;
    mat.m[5] = f;
    mat.m[10] = (near + far) / (near - far);
    mat.m[11] = (2.0f * near * far) / (near - far);
    mat.m[14] = -1.0f;
    mat.m[15] = 0.0f;
    return mat;
}

Mat4 Mat4::look_at(const Vec3& eye, const Vec3& target, const Vec3& up) {
    Vec3 forward = (target - eye).normalized();
    Vec3 right = forward.cross(up).normalized();
    Vec3 new_up = right.cross(forward);

    Mat4 mat;
    mat.m[0] = right.x;
    mat.m[1] = right.y;
    mat.m[2] = right.z;
    mat.m[3] = -right.dot(eye);
    mat.m[4] = new_up.x;
    mat.m[5] = new_up.y;
    mat.m[6] = new_up.z;
    mat.m[7] = -new_up.dot(eye);
    mat.m[8] = -forward.x;
    mat.m[9] = -forward.y;
    mat.m[10] = -forward.z;
    mat.m[11] = forward.dot(eye);
    return mat;
}

Mat4 Mat4::translation(const Vec3& pos) {
    Mat4 mat;
    mat.m[12] = pos.x;
    mat.m[13] = pos.y;
    mat.m[14] = pos.z;
    return mat;
}

Mat4 Mat4::scale(const Vec3& s) {
    Mat4 mat;
    mat.m[0] = s.x;
    mat.m[5] = s.y;
    mat.m[10] = s.z;
    return mat;
}

Quat Quat::from_euler(float pitch, float yaw, float roll) {
    float cy = std::cos(yaw * 0.5f);
    float sy = std::sin(yaw * 0.5f);
    float cp = std::cos(pitch * 0.5f);
    float sp = std::sin(pitch * 0.5f);
    float cr = std::cos(roll * 0.5f);
    float sr = std::sin(roll * 0.5f);

    return Quat(
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    );
}

Vec3 Quat::to_euler() const {
    // Roll, pitch, yaw
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    float roll = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (w * y - z * x);
    float pitch = std::abs(sinp) >= 1.0f 
        ? std::copysign(M_PI / 2.0f, sinp) 
        : std::asin(sinp);

    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    float yaw = std::atan2(siny_cosp, cosy_cosp);

    return {pitch, yaw, roll};
}

} // namespace kow