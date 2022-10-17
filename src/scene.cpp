#include "scene.h"

#include "draw.h"
#include "phong.h"
#include "bmp.h"

const Material material {
    0.1f,
    0.8f,
    0.5f,
    8.0f,
};

const float t = 1e-2f;

const Model square(
    {
        Vertex { Vec3( t,  t, 0), Vec2(), Vec3() },
        Vertex { Vec3(-t,  t, 0), Vec2(), Vec3() },
        Vertex { Vec3(-t, -t, 0), Vec2(), Vec3() },
        Vertex { Vec3( t, -t, 0), Vec2(), Vec3() },
    },
    {
        Face { 0, 1, 2 },
        Face { 2, 3, 0 },
    }
);

void DrawLight(Framebuffer<Vec4>& framebuffer, const Light& light, const Mat4& proj) {
    auto vertex = [&](const Vertex& v) {
        return std::tuple(proj * Vec4(light.position + v.position, 1));
    };

    auto fragment = [&] {
        return Vec4(light.colour, 1);
    };

    Draw(framebuffer, vertex, fragment, square.faces, square.vertices);
}

Mat4 Perspective(float w, float h) {
    float fov    = M_PIf / 2;
    float aspect = w / h;
    float near   = 1e-2f;
    float far    = 1e2f;

    return Perspective(fov, aspect, near, far);
}

void Scene::Draw(Framebuffer<Vec4>& framebuffer, float time) {
    auto mv = Translate(0, 0, 1) * RotateY(time);
    auto proj = Perspective(framebuffer.Width(), framebuffer.Height());

    for (int i = 0; auto& light : this->lights) {
        float t = 3.14f * std::cos(i++) * time;

        light.position.x = std::cos(t);
        light.position.z = std::sin(t);

        DrawLight(framebuffer, light, proj);
    }

    for (auto& [model, texture] : this->objects) {
        DrawPhongShaded(framebuffer, model, texture, material, this->lights, mv, proj);
    }
}

Scene Scene::LoadDemoScene(const std::string& model_path, const std::string& texture_path) {
    Scene result;

    result.objects.emplace_back(Object {
        Model::Load(model_path), LoadTexture(texture_path)
    });

    result.lights.emplace_back(Light { Vec3(), Vec3(1, 0.2, 0.1) });
    result.lights.emplace_back(Light { Vec3(), Vec3(0.1, 0.2, 1) });

    return result;
}
