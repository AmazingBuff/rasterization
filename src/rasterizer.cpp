//
// Created by AmazingBuff on 25-7-25.
//

RASTERIZER_NAME_SPACE_BEGIN

struct EarlyZInfo
{
    bool depth_test_passed;
    uint32_t triangle_index;
    vec3 barycentric;
};

Pixel apply_shading(Triangle const& triangle, vec3 const& barycentric, Shading const& shading)
{
    PixelIn pixel_in{
        .pos = barycentric.x() * triangle.v0.pixel_in.pos + barycentric.y() * triangle.v1.pixel_in.pos + barycentric.z() * triangle.v2.pixel_in.pos,
        .uv = barycentric.x() * triangle.v0.pixel_in.uv + barycentric.y() * triangle.v1.pixel_in.uv + barycentric.z() * triangle.v2.pixel_in.uv,
        .normal = barycentric.x() * triangle.v0.pixel_in.normal + barycentric.y() * triangle.v1.pixel_in.normal + barycentric.z() * triangle.v2.pixel_in.normal,
        .tangent = barycentric.x() * triangle.v0.pixel_in.tangent + barycentric.y() * triangle.v1.pixel_in.tangent + barycentric.z() * triangle.v2.pixel_in.tangent,
    };

    vec4 color = remap(clamp(shading(pixel_in), 0.0, 1.0), 0.0, 1.0, 0.0, 255.0);
    return {static_cast<uint8_t>(color.x()), static_cast<uint8_t>(color.y()), static_cast<uint8_t>(color.z()), static_cast<uint8_t>(color.w())};
}

Pixel pixel_shading(ShadingDescriptor const& descriptor, Vector<Triangle> const& triangles, Shading const& shading, vec2 const& p)
{
    Pixel pixel = {0, 0, 0, 0};

    Float z_max = descriptor.reverse_z ? 0.0f : 1.0f;
    EarlyZInfo early_z_info{
        .depth_test_passed = false,
        .triangle_index = 0,
        .barycentric = vec3(0.0f, 0.0f, 0.0f)
    };
    for (uint32_t i = 0; i < triangles.size(); i++)
    {
        Triangle const& triangle = triangles[i];

        vec2 p0(triangle.v0.position.x(), triangle.v0.position.y());
        vec2 p1(triangle.v1.position.x(), triangle.v1.position.y());
        vec2 p2(triangle.v2.position.x(), triangle.v2.position.y());

        vec2 p10 = p1 - p0;
        vec2 p21 = p2 - p1;
        vec2 p02 = p0 - p2;
        vec2 pp0 = p - p0;
        vec2 pp1 = p - p1;
        vec2 pp2 = p - p2;

        vec3 area = vec3(cross(p10, pp0), cross(p21, pp1), cross(p02, pp2));

        if (area.x() >= 0 && area.y() >= 0 && area.z() >= 0)
        {
            // barycentric coordinates
            vec3 barycentric = vec3(area.y(), area.z(), area.x()) / (area.x() + area.y() + area.z());

            Float z = triangle.v0.position.z() * barycentric.x() + triangle.v1.position.z() * barycentric.y() + triangle.v2.position.z() * barycentric.z();

            if ((descriptor.reverse_z && z > z_max && z < 1.0) ||
                (!descriptor.reverse_z && z < z_max && z > 0.0))
            {
                z_max = z;
                if (descriptor.early_z)
                {
                    early_z_info.depth_test_passed = true;
                    early_z_info.triangle_index = i;
                    early_z_info.barycentric = barycentric;
                }
                else
                    pixel = apply_shading(triangle, barycentric, shading);
            }
        }
    }

    if (descriptor.early_z && early_z_info.depth_test_passed)
    {
        Triangle const& triangle = triangles[early_z_info.triangle_index];
        vec3 const& barycentric = early_z_info.barycentric;

        pixel = apply_shading(triangle, barycentric, shading);
    }

    return pixel;
}

PixelFrame rasterize(RasterizerDescriptor const& descriptor)
{
    Vector<Triangle> triangles;
    if (!descriptor.mesh.indices.empty())
    {
        triangles.resize(descriptor.mesh.indices.size());
        Vector<VertexOut> out_vertices(descriptor.mesh.vertices.size());
        for_each(ParallelStrategy::e_parallel, descriptor.mesh.vertices, [&](Vertex const& vertex)
        {
            uint32_t i = &vertex - descriptor.mesh.vertices.data();
            PixelIn in;
            vec4 pos = descriptor.vertexing(vertex, in);
            // to NDC, w don't need to change
            pos.x() = pos.x() / pos.w();
            pos.y() = pos.y() / pos.w();
            pos.z() = pos.z() / pos.w();
            if (descriptor.rasterizer_state.reverse_z)
                pos.z() = 1 - pos.z();

            out_vertices[i] = {pos, in};
        });

        for_each(ParallelStrategy::e_parallel, triangles, [&](Triangle& triangle)
        {
            uint32_t i = &triangle - triangles.data();
            triangle.v0 = out_vertices[descriptor.mesh.indices[i].x()];
            triangle.v1 = out_vertices[descriptor.mesh.indices[i].y()];
            triangle.v2 = out_vertices[descriptor.mesh.indices[i].z()];
        });
    }
    else
    {
        triangles.resize(descriptor.mesh.vertices.size() / 3);
        Vector<VertexOut> out_vertices(descriptor.mesh.vertices.size());
        for_each(ParallelStrategy::e_parallel, descriptor.mesh.vertices, [&](Vertex const& vertex)
        {
            uint32_t i = &vertex - descriptor.mesh.vertices.data();
            PixelIn in;
            vec4 pos = descriptor.vertexing(vertex, in);
            // to NDC, w don't need to change
            pos.x() = pos.x() / pos.w();
            pos.y() = pos.y() / pos.w();
            pos.z() = pos.z() / pos.w();
            if (descriptor.rasterizer_state.reverse_z)
                pos.z() = 1 - pos.z();

            out_vertices[i] = {pos, in};
        });

        for_each(ParallelStrategy::e_parallel, triangles, [&](Triangle& triangle)
        {
            uint32_t i = &triangle - triangles.data();
            triangle.v0 = out_vertices[i * 3 + 0];
            triangle.v1 = out_vertices[i * 3 + 1];
            triangle.v2 = out_vertices[i * 3 + 2];
        });
    }


    PixelFrame frame{
        .width = descriptor.width,
        .height = descriptor.height,
    };
    frame.pixels.resize(frame.width * frame.height);

    ShadingDescriptor shading_descriptor{
        .early_z = descriptor.rasterizer_state.early_z,
        .reverse_z = descriptor.rasterizer_state.reverse_z,
    };

    for_each(ParallelStrategy::e_parallel, frame.pixels, [&](Pixel& pixel)
    {
        uint32_t x = (&pixel - frame.pixels.data()) % frame.width;
        uint32_t y = (&pixel - frame.pixels.data()) / frame.width;
        if (descriptor.rasterizer_state.flip_y)
            y = frame.height - 1 - y;

        // [-1, 1]
        vec2 p(static_cast<Float>(x) / static_cast<Float>(frame.width) * 2.0 - 1.0, static_cast<Float>(y) / static_cast<Float>(frame.height) * 2.0 - 1.0);

        frame.pixels[y * frame.width + x] = pixel_shading(shading_descriptor, triangles, descriptor.shading, p);
    });

    return frame;
}

RASTERIZER_NAME_SPACE_END