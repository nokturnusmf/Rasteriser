#pragma once

#include <vector>
#include <string>

#include "matrix.h"

struct Vertex {
    Vec3 position;
    Vec2 tex_coord;
    Vec3 normal;
};

struct Face {
    int v1;
    int v2;
    int v3;
};

struct Model {
    Model(std::vector<Vertex> vertices, std::vector<Face> faces);

    static Model Load(const std::string& path);
    void Save(const std::string& path);

    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

Mat4 Recenter(const Model& model);

void CalculateVertexNormals(Model& model);
