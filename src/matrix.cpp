#include "matrix.h"

float Dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Cross(Vec3 a, Vec3 b) {
    return Vec3(
        a.z * b.y - a.y * b.z,
        a.x * b.z - a.z * b.x,
        a.y * b.x - a.x * b.y
    );
}

Vec3 Normal(Vec3 a, Vec3 b, Vec3 c) {
    return Cross(b - a, c - a);
}

float Length(Vec3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 Normalize(Vec3 v) {
    return v / Length(v);
}

Vec3 Min(Vec3 a, Vec3 b) {
    return Vec3(
        std::min(a.x, b.x),
        std::min(a.y, b.y),
        std::min(a.z, b.z)
    );
}

Vec3 Max(Vec3 a, Vec3 b) {
    return Vec3(
        std::max(a.x, b.x),
        std::max(a.y, b.y),
        std::max(a.z, b.z)
    );
}

IVec2 Min(IVec2 a, IVec2 b) {
    return IVec2(
        std::min(a.x, b.x),
        std::min(a.y, b.y)
    );
}

IVec2 Max(IVec2 a, IVec2 b) {
    return IVec2(
        std::max(a.x, b.x),
        std::max(a.y, b.y)
    );
}

Mat4 Translate(float x, float y, float z) {
    // return Mat4(
    //     1, 0, 0, x,
    //     0, 1, 0, y,
    //     0, 0, 1, z,
    //     0, 0, 0, 1
    // );

    return Mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    );
}

Mat4 Scale(float s) {
    return Mat4(
        s, 0, 0, 0,
        0, s, 0, 0,
        0, 0, s, 0,
        0, 0, 0, 1
    );
}

Mat4 RotateX(float radians) {
    auto s = std::sin(radians);
    auto c = std::cos(radians);

    return Mat4(
        1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1
    );
}

Mat4 RotateY(float radians) {
    auto s = std::sin(radians);
    auto c = std::cos(radians);

    return Mat4(
        c, 0,-s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1
    );
}

Mat4 Perspective(float fov, float aspect, float near, float far) {
    auto tan_fov = std::tan(fov / 2);

    float y = 1 / tan_fov;
    float x = y / aspect;

    float z = far / (far - near);
    float w = -(far * near) / (far - near);

    return Mat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 1,
        0, 0, w, 0
    );
}
