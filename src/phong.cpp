#include "phong.h"

#include <tuple>

#include "draw.h"
#include "lighting.h"

void DrawPhongShaded(Framebuffer<Vec4>& framebuffer, const Model& model, const Texture<Vec3>& texture, const Material& material, std::span<const Light> lights, const Mat4& mv, const Mat4& proj) {
    auto vertex = [&](const Vertex& v) {
        Vec4 position  = mv * Vec4(v.position, 1);
        Vec4 clip      = proj * position;
        Vec2 tex_coord = v.tex_coord;
        Vec3 normal    = mv * Vec4(v.normal, 0);

        return std::tuple(clip, position, tex_coord, normal);
    };

    auto pixel = [&](const Vec4& position, const Vec2& tex_coord, const Vec3& normal) {
        auto colour = texture.Sample(tex_coord.y, tex_coord.x);
        auto result = PhongIllumination(position, normal, colour, material, lights);

        return Vec4(result, 1);
    };

    Draw(framebuffer, vertex, pixel, model.faces, model.vertices);
}
