#pragma once

#include <span>

#include "matrix.h"
#include "model.h"
#include "framebuffer.h"
#include "lighting.h"

void DrawPhongShaded(Framebuffer<Vec4>& framebuffer, const Model& model, const Texture<Vec3>& texture, const Material& material, std::span<const Light> lights, const Mat4& mv, const Mat4& proj);
