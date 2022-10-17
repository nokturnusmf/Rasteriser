#include "model.h"
#include "matrix.h"
#include "io.h"

#include <span>

Model::Model(std::vector<Vertex> vertices, std::vector<Face> faces) {
    this->vertices = std::move(vertices);
    this->faces = std::move(faces);
}

Model Model::Load(const std::string& path) {
    auto file = OpenFileRead(path);

    auto vertices_count = Read<size_t>(file);
    auto faces_count = Read<size_t>(file);

    auto vertices = ReadVec<Vertex>(file, vertices_count);
    auto faces = ReadVec<Face>(file, faces_count);

    return Model(std::move(vertices), std::move(faces));
}

void Model::Save(const std::string& path) {
    auto file = OpenFileWrite(path);

    Write<size_t>(file, vertices.size());
    Write<size_t>(file, faces.size());

    WriteVec(file, vertices);
    WriteVec(file, faces);
}

Mat4 Recenter(std::span<const Vec3> vertices) {
    // Calculate the mean of all vertices, and translate in the opposite direction.
    Vec3 mean(0, 0, 0);

    for (auto v : vertices) {
        mean += v;
    }

    mean /= vertices.size();

    return Translate(-mean.x, -mean.y, -mean.z);
}

Mat4 Rescale(std::span<const Vec3> vertices) {
    // Calculate the maximum and minimum coordinates for each component.
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(std::numeric_limits<float>::lowest());

    for (auto v : vertices) {
        min = Min(v, min);
        max = Max(v, max);
    }

    // Calculate the maximum size in any single direction.
    auto size = max - min;
    auto m = std::max(size.x, std::max(size.y, size.z));

    // Rescale that size to be between -1 and 1.
    return Scale(2 / m);
}

Mat4 Recenter(const Model& model) {
    std::vector<Vec3> vs;
    vs.reserve(model.vertices.size());

    for (auto& v : model.vertices) {
        vs.emplace_back(v.position);
    }

    return Rescale(vs) * Recenter(vs);
}

void CalculateVertexNormals(Model& model) {
    for (auto& vertex : model.vertices) {
        vertex.normal = Vec3();
    }

    for (auto& face : model.faces) {
        auto normal = Normal(
            model.vertices[face.v1].position,
            model.vertices[face.v2].position,
            model.vertices[face.v3].position
        );

        model.vertices[face.v1].normal += normal;
        model.vertices[face.v2].normal += normal;
        model.vertices[face.v3].normal += normal;
    }

    for (auto& vertex : model.vertices) {
        vertex.normal = Normalize(vertex.normal);
    }
}
