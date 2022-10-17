#pragma once

#include <memory>
#include <cmath>

template<typename T>
class Texture {
public:
    Texture(int w, int h);

    int Width() const;
    int Height() const;

    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    T Sample(float y, float x) const;

    void Clear(T value = T());

    const T* Data() const;

private:
    int Index(int row, int col) const;

    std::unique_ptr<T[]> data;

    int width;
    int height;
};

template<typename T>
Texture<T>::Texture(int w, int h) {
    this->width = w;
    this->height = h;

    this->data = std::make_unique<T[]>(width * height);
}

template<typename T>
int Texture<T>::Width() const {
    return this->width;
}

template<typename T>
int Texture<T>::Height() const {
    return this->height;
}

template<typename T>
T& Texture<T>::operator()(int row, int col) {
    return this->data[this->Index(row, col)];
}

template<typename T>
const T& Texture<T>::operator()(int row, int col) const {
    return this->data[this->Index(row, col)];
}

template<typename T>
T Texture<T>::Sample(float y, float x) const {
    constexpr float eps = 1e-3f;

    x = x * (width - eps);
    y = y * (height - eps);

    return (*this)(y, x);
}

template<typename T>
void Texture<T>::Clear(T value) {
    for (int i = 0; i < width * height; ++i) {
        this->data[i] = value;
    }
}

template<typename T>
const T* Texture<T>::Data() const {
    return this->data.get();
}

template<typename T>
int Texture<T>::Index(int row, int col) const {
    return row * this->width + col;
}
