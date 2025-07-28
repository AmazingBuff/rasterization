//
// Created by AmazingBuff on 25-7-27.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "internal/mathematics.h"

RASTERIZER_NAME_SPACE_BEGIN

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Texture
{
    Vector<Pixel> pixels;
    uint32_t width;
    uint32_t height;
    uint32_t mip_levels;
};

struct PixelFrame
{
    Pixel* pixels;
    uint32_t width;
    uint32_t height;
};

PixelFrame mip_frame(Texture const& texture, uint32_t level);
Pixel bilinear_lod(Texture const& texture, vec2 uv);

RASTERIZER_NAME_SPACE_END

#endif //TEXTURE_H
