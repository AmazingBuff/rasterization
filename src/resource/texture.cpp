//
// Created by AmazingBuff on 25-7-27.
//

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

}

RASTERIZER_NAME_SPACE_END