#pragma once

#include <span>
#include <queue>

#include "matrix.h"
#include "model.h"
#include "framebuffer.h"
#include "tuple.h"

inline IVec2 MapToScreen(Vec2 v, int height, int width) {
    return IVec2(
        (v.x + 1) / 2 * width  + 0.5f,
        (v.y + 1) / 2 * height + 0.5f
    );
}

template<typename Tuple>
auto& Position(Tuple&& tuple) {
    return Get<0>(std::forward<Tuple>(tuple));
}

template<size_t I, typename V>
auto Interpolate(const V& v1, const V& v2, const V& v3, float w1, float w2, float w3) {
    return w1 * Get<I>(v1) + w2 * Get<I>(v2) + w3 * Get<I>(v3);
}

template<typename V, size_t... I>
auto Interpolate(const V& v1, const V& v2, const V& v3, float w1, float w2, float w3, std::index_sequence<I...>) {
    return std::tuple(Interpolate<I>(v1, v2, v3, w1, w2, w3)...);
}

template<typename V>
auto Interpolate(const V& v1, const V& v2, const V& v3, float w1, float w2, float w3) {
    return Interpolate(v1, v2, v3, w1, w2, w3, std::make_index_sequence<std::tuple_size_v<V>>{});
}

inline std::tuple<float, float, float> AdjustInterpolation(const Vec4& p1, const Vec4& p2, const Vec4& p3, float w1, float w2, float w3) {
    w1 *= p1.w;
    w2 *= p2.w;
    w3 *= p3.w;

    auto s = 1 / (w1 + w2 + w3);

    return std::tuple(w1 * s, w2 * s, w3 * s);
}

template<typename... P, typename V, class Pixel>
void Rasterize(Framebuffer<P...>& framebuffer, const V& v1, const V& v2, const V& v3, Pixel pixel) {
    auto& p1 = Position(v1);
    auto& p2 = Position(v2);
    auto& p3 = Position(v3);

    // Extract data to be interpolated.
    auto d1 = Tail(v1);
    auto d2 = Tail(v2);
    auto d3 = Tail(v3);

    auto h = framebuffer.Height();
    auto w = framebuffer.Width();

    auto s1 = MapToScreen(p1, h, w);
    auto s2 = MapToScreen(p2, h, w);
    auto s3 = MapToScreen(p3, h, w);

    auto det = (s2.y - s3.y) * (s1.x - s3.x) + (s3.x - s2.x) * (s1.y - s3.y);
    if (det == 0) return;

    auto scale = 1.f / det;

    // Calculate the region of pixels containing the triangle.
    auto min = Min(s1, Min(s2, s3));
    auto max = Max(s1, Max(s2, s3));

    // Only calculate on screen pixels.
    min = Max(min, IVec2(0, 0));
    max = Min(max, IVec2(w - 1, h - 1));

    thread_local std::queue<std::tuple<int, int, float, float, float, float>> queue;

    auto Emit = [&](int x, int y, float z, float w1, float w2, float w3) {
        // Early (unlocked) depth test.
        if (!framebuffer.DepthTest(x, y, z)) return;

        if (auto lock = framebuffer.Lock(x, y)) {
            // Recheck depth with lock held.
            if (!framebuffer.DepthTest(x, y, z)) return;

            // Calculate pixel value.
            auto v = Interpolate(d1, d2, d3, w1, w2, w3);
            framebuffer.Set(x, y, z, std::apply(pixel, v));
        } else {
            // Couldn't get lock for this pixel, so return it to the back of the queue.
            queue.emplace(x, y, z, w1, w2, w3);
        }
    };

    for (int y = min.y; y <= max.y; ++y) {
        for (int x = min.x; x <= max.x; ++x) {
            auto w1 = ((s2.y - s3.y) * (x - s3.x) + (s3.x - s2.x) * (y - s3.y)) * scale;
            auto w2 = ((s3.y - s1.y) * (x - s3.x) + (s1.x - s3.x) * (y - s3.y)) * scale;
            auto w3 = 1 - w1 - w2;

            // Ensure this point is within the triangle.
            if (w1 < 0 || w2 < 0 || w3 < 0) continue;

            auto [w1_, w2_, w3_] = AdjustInterpolation(p1, p2, p3, w1, w2, w3);
            auto z = w1_ * p1.z + w2_ * p2.z + w3_ * p3.z;

            Emit(x, y, z, w1_, w2_, w3_);
        }
    }

    // Clear any remaining items in the queue.
    while (!queue.empty()) {
        auto x = queue.front();
        queue.pop();

        std::apply(Emit, x);
    }
}

inline float Facing(Vec2 a, Vec2 b, Vec2 c) {
    // Calculate only the Z component of the cross product of AC and AB.
    auto ab = b - a;
    auto ac = c - a;

    return ac.x * ab.y - ac.y * ab.x;
}

template<typename V>
auto& ApplyPerspectiveDivide(V& v) {
    auto& position = Position(v);

    float w = 1 / position.w;

    position.x *= w;
    position.y *= w;
    position.z *= w;
    position.w  = w;

    return position;
}

template<typename... P, class Vertex, class Pixel, class... Args>
void Draw(Framebuffer<P...>& framebuffer, Vertex vertex, Pixel pixel, std::span<const Face> faces, Args&&... args) {
    #pragma omp parallel for
    for (auto& face : faces) {
        auto v1 = vertex(args[face.v1]...);
        auto v2 = vertex(args[face.v2]...);
        auto v3 = vertex(args[face.v3]...);

        auto p1 = ApplyPerspectiveDivide(v1);
        auto p2 = ApplyPerspectiveDivide(v2);
        auto p3 = ApplyPerspectiveDivide(v3);

        // Only add triangles which are facing the camera.
        if (Facing(p1, p2, p3) < 0) {
            Rasterize(framebuffer, v1, v2, v3, pixel);
        }
    }
}
