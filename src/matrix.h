#pragma once

#include <glm/matrix.hpp>

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using IVec2 = glm::ivec2;

using Mat4 = glm::mat4;

float Dot(Vec3 a, Vec3 b);
Vec3 Cross(Vec3 a, Vec3 b);

Vec3 Normal(Vec3 a, Vec3 b, Vec3 c);

float Length(Vec3 v);
Vec3 Normalize(Vec3 v);

Vec3 Min(Vec3 a, Vec3 b);
Vec3 Max(Vec3 a, Vec3 b);

IVec2 Min(IVec2 a, IVec2 b);
IVec2 Max(IVec2 a, IVec2 b);

Mat4 Translate(float x, float y, float z);
Mat4 Scale(float s);
Mat4 RotateX(float radians);
Mat4 RotateY(float radians);

Mat4 Perspective(float fov, float aspect, float near, float far);
