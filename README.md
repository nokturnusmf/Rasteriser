# Rasteriser

A software rasteriser, written in C++.

## Features

- Multithreaded rendering
- Fully programmable vertex and pixel shaders without virtual function calls
- Perspective correct interpolation
- Back face culling
- Texturing

## Example Rendering

![Lucy](https://user-images.githubusercontent.com/10729925/196252469-40fc87b7-e31b-4231-bdb9-373b81d1b07c.png)

Model from [the Stanford 3D Scanning Repository](https://graphics.stanford.edu/data/3Dscanrep/). This model contains ~28,000,000 triangles, and renders in ~140ms at 1920x1080 with a Ryzen 5800H.

See the releases section to download this and some other example models.

## Compiling and Running

This project uses CMake. The simplest option is to run the included build script.

This project uses the following:
- [GLM](https://github.com/g-truc/glm) for some mathematical operations
- [GLFW](https://www.glfw.org/) to create the display window
- (Legacy) OpenGL to display the rendered result only
- (Optionally) OpenMP for multithreading

Run the compiled program with:

```build/src/rasterizer <path/to/model> <path/to/texture>```

## Programmable Shaders Example

Both the vertex and pixel shaders are fully programmable. The vertex shader returns a tuple of clip coordinates along with any other data. Each element of this extra data is automatically interpolated and passed to the pixel shader as a parameter.

```cpp
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
```
