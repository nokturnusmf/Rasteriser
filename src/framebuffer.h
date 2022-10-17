#pragma once

#include <tuple>
#include <optional>

#include "texture.h"
#include "concurrency.h"

template<class... Types>
class Framebuffer {
public:
    Framebuffer(int w, int h, bool depth, int tile_size = 0);

    void Set(int x, int y, float depth, std::tuple<Types...>&& value);

    bool DepthTest(int x, int y, float depth) const;
    SimpleLock Lock(int x, int y);

    void Clear();

    int Width()  const { return this->data.Width();  }
    int Height() const { return this->data.Height(); }

private:
    friend class Window;
    const void* Raw() const { return &data(0, 0); }

    Texture<std::tuple<Types...>> data;
    Texture<SimpleMutex> lock;
    std::optional<Texture<float>> depth;

    int tile_size;
};

template<class... Types>
Framebuffer<Types...>::Framebuffer(int w, int h, bool depth, int tile_size) : data(w, h), lock(w >> tile_size, h >> tile_size) {
    if (depth) {
        this->depth = Texture<float>(w, h);
    }

    this->tile_size = tile_size;
}

template<class... Types>
void Framebuffer<Types...>::Set(int x, int y, float depth, std::tuple<Types...>&& value) {
    if (this->depth) {
        (*this->depth)(y, x) = depth;
    }

    this->data(y, x) = std::move(value);
}

template<class... Types>
bool Framebuffer<Types...>::DepthTest(int x, int y, float depth) const {
    if (!this->depth) return true;
    if (depth < 0 || depth > 1) return false;

    return depth < (*this->depth)(y, x);
}

template<class... Types>
SimpleLock Framebuffer<Types...>::Lock(int x, int y) {
    return SimpleLock(this->lock(y >> tile_size, x >> tile_size));
}

template<class... Types>
void Framebuffer<Types...>::Clear() {
    this->data.Clear();

    if (this->depth) this->depth->Clear(1e9);
}
