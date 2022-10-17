#pragma once

#include <string>

#include "matrix.h"
#include "texture.h"

Texture<Vec3> LoadTexture(const std::string& path);
