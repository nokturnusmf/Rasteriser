#include "lighting.h"

#include <cmath>

// These functions assume the viewing direction is already aligned to the Z-axis.

Vec3 PhongIllumination(Vec3 position, Vec3 normal, Vec3 colour, const Material& material, const Light& light) {
    auto lm = Normalize(light.position - position);

    auto diffuse = Dot(lm, normal);
    if (diffuse < 0) return Vec3(0);

    auto rm = 2 * diffuse * normal - lm;
    auto specular = rm.z > 0 ? std::pow(rm.z, material.shininess) : 0;

    auto scale = diffuse * material.diffuse + specular * material.specular;
    return scale * light.colour * colour;
}

Vec3 PhongIllumination(Vec3 position, Vec3 normal, Vec3 colour, const Material& material, std::span<const Light> lights) {
    Vec3 result = colour * material.ambient;

    // Add the contribution from each light.
    for (auto& light : lights) {
        result += PhongIllumination(position, normal, colour, material, light);
    }

    return result;
}
