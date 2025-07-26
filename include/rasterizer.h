//
// Created by AmazingBuff on 25-7-25.
//

#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "internal/mathematics.h"

RASTERIZER_NAME_SPACE_BEGIN

struct Vertex
{
    vec3 position;
    vec2 tex_coord;
    vec3 normal;
    vec3 tangent;
};

struct Mesh
{
    Vector<Vertex> vertices;
    Vector<ivec3> indices;
};

struct PixelIn
{
    vec3 pos;
    vec2 uv;
    vec3 normal;
    vec3 tangent;
};

using Vertexing = Functional<vec4(Vertex const&, PixelIn&)>;
using Shading = Functional<vec4(PixelIn const&)>;

struct RasterizerDescriptor
{
    struct
    {
        uint32_t flip_y : 1;
        uint32_t early_z : 1;
    } rasterizer_state;

    uint32_t width;
    uint32_t height;
    Mesh mesh;
    Vertexing vertexing;
    Shading shading;
};

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct PixelFrame
{
    Vector<Pixel> pixels;
    uint32_t width;
    uint32_t height;
};

PixelFrame rasterize(RasterizerDescriptor const& descriptor);

RASTERIZER_NAME_SPACE_END

#endif //RASTERIZER_H
