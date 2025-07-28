//
// Created by AmazingBuff on 25-7-25.
//

#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "internal/mathematics.h"
#include "resource/texture.h"

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

struct VertexingBuiltinIn
{
    uint32_t vertex_index;
    uint32_t instance_index;
};

struct ShadingBuiltinOut
{
    Float depth;
};

using Vertexing = Functional<vec4(Vertex const&, VertexingBuiltinIn const&, PixelIn&)>;
using Shading = Functional<vec4(PixelIn const&, ShadingBuiltinOut&)>;

struct RasterizerDescriptor
{
    struct
    {
        uint32_t flip_y : 1;
        uint32_t early_z : 1;
        uint32_t reverse_z : 1;
    } rasterizer_state;

    uint32_t width;
    uint32_t height;
    Mesh mesh;
    Vertexing vertexing;
    Shading shading;
};

Vector<Mesh> load_mesh(String const& path);
Texture load_image(String const& path);
Texture rasterize(RasterizerDescriptor const& descriptor);

RASTERIZER_NAME_SPACE_END

#endif //RASTERIZER_H
