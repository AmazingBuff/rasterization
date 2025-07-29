//
// Created by AmazingBuff on 25-7-27.
//


namespace std
{
    template <typename T>
    Amazing::Rasterizer::Pixel lerp(Amazing::Rasterizer::Pixel const& l, Amazing::Rasterizer::Pixel const& r, T const weight)
    {
        return {
            .r = static_cast<uint8_t>(std::lerp(static_cast<T>(l.r), static_cast<T>(r.r), weight)),
            .g = static_cast<uint8_t>(std::lerp(static_cast<T>(l.g), static_cast<T>(r.g), weight)),
            .b = static_cast<uint8_t>(std::lerp(static_cast<T>(l.b), static_cast<T>(r.b), weight)),
            .a = static_cast<uint8_t>(std::lerp(static_cast<T>(l.a), static_cast<T>(r.a), weight)),
        };
    }
}

RASTERIZER_NAME_SPACE_BEGIN

PixelFrame mip_frame(Texture const& texture, uint32_t level)
{
    ASSERT(level < texture.mip_levels, "frame", "frame level out of range!");\

    uint32_t offset = 0;
    uint32_t width = texture.width;
    uint32_t height = texture.height;
    for (uint32_t i = 0; i < level; ++i)
    {
        width = std::max(texture.width >> i, 1u);
        height = std::max(texture.height >> i, 1u);
        offset += width * height;
    }

    PixelFrame frame{
        .pixels = const_cast<Pixel*>(texture.pixels.data() + offset),
        .width = width,
        .height = height,
    };

    return frame;
}

Pixel bilinear_lod(Texture const& texture, vec2 uv)
{
    const Float x = uv.x() * static_cast<Float>(texture.width);
    const Float y = uv.y() * static_cast<Float>(texture.height);

    Pixel pixel{};
    if (texture.mip_levels > 1)
    {

    }
    else
    {
        const uint32_t x0 = std::max(std::min(static_cast<uint32_t>(x), texture.width - 1), 0u);
        const uint32_t y0 = std::max(std::min(static_cast<uint32_t>(y), texture.height - 1), 0u);
        const uint32_t x1 = x0 >= texture.width - 1 ? x0 : x0 + 1;
        const uint32_t y1 = y0 >= texture.height - 1 ? y0 : y0 + 1;

        const Float weight_x = x - static_cast<Float>(x0);
        const Float weight_y = y - static_cast<Float>(y0);

        const Pixel lt = texture.pixels[y0 * texture.width + x0];
        const Pixel rt = texture.pixels[y0 * texture.width + x1];
        const Pixel lb = texture.pixels[y1 * texture.width + x0];
        const Pixel rb = texture.pixels[y1 * texture.width + x1];

        pixel = std::lerp(std::lerp(lt, rt, weight_x), std::lerp(lb, rb, weight_x), weight_y);
    }

    return pixel;
}

RASTERIZER_NAME_SPACE_END