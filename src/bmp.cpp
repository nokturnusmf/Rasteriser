#include "bmp.h"

#include "io.h"

#pragma pack(1)
struct BMPHeader {
    // header
    uint16_t id = 0x4D42;
    uint32_t size;
    uint16_t reserved0 = 0;
    uint16_t reserved1 = 0;
    uint32_t offset;
    // bitmapinfoheader
    uint32_t bitmapinfoheader_size = 40;
    int32_t x;
    int32_t y;
    uint16_t planes = 1;
    uint16_t depth;
    uint32_t compression = 0;
    uint32_t pixels_size;
    int32_t horizontal_res = 2835;
    int32_t vertical_res = 2835;
    uint32_t palette = 0;
    uint32_t important = 0;
};

Texture<Vec3> LoadTexture(const std::string& path) {
    auto file = OpenFileRead(path);

    auto header = Read<BMPHeader>(file);

    auto size = header.x * header.y * 3;
    auto raw = ReadVec<unsigned char>(file, size);

    Texture<Vec3> result(header.x, header.y);

    for (int i = 0; i < header.y; ++i) {
        for (int j = 0; j < header.x; ++j) {
            auto& pixel = result(i, j);

            auto k = 3 * (i * header.x + j);
            pixel.r = raw[k + 2] / 255.f;
            pixel.g = raw[k + 1] / 255.f;
            pixel.b = raw[k + 0] / 255.f;
        }
    }

    return result;
}
