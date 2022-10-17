#pragma once

#include <span>

#include "matrix.h"

struct Material {
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

struct Light {
    Vec3 position;
    Vec3 colour;
};

Vec3 PhongIllumination(Vec3 position, Vec3 normal, Vec3 colour, const Material& material, std::span<const Light> lights);
