#pragma once

#include "model.h"
#include "texture.h"
#include "lighting.h"
#include "framebuffer.h"

struct Object {
    Model model;
    Texture<Vec3> texture;
};

class Scene {
public:
    static Scene LoadDemoScene(const std::string& model_path, const std::string& texture_path);

    void Draw(Framebuffer<Vec4>& framebuffer, float time);

private:
    std::vector<Object> objects;
    std::vector<Light> lights;
};
