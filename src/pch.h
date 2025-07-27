//
// Created by AmazingBuff on 25-7-25.
//

#ifndef PCH_H
#define PCH_H

#include "rasterizer.h"

RASTERIZER_NAME_SPACE_BEGIN

struct VertexOut
{
    vec4 position;      // gl_position
    PixelIn pixel_in;   // pixel input
};

struct Triangle
{
    VertexOut v0, v1, v2;
};

struct ShadingDescriptor
{
    uint32_t early_z : 1;
    uint32_t reverse_z : 1;
};


RASTERIZER_NAME_SPACE_END
#endif //PCH_H
