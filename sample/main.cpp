//
// Created by AmazingBuff on 25-7-25.
//

#include <rasterizer.h>
#include <chrono>
#include <SDL3/SDL.h>

using namespace Amazing;
using namespace Amazing::Rasterizer;

constexpr static uint32_t Width = 960;
constexpr static uint32_t Height = 540;

constexpr Float fov = 45.0;
constexpr Float z_near = 0.1;
constexpr Float z_far = 100.0;

int main()
{
    mat4 model = translate(vec3(0, 0.5, -2)) * rotate(vec3(0, 1, 0), 90) * rotate(vec3(01, 0, 0), 90);
    mat4 view = look_at(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
    mat4 projection = perspective(fov, static_cast<Float>(Width) / static_cast<Float>(Height), z_near, z_far);

    //Vector<Mesh> meshes = load_mesh("E:/code/github/rasterization/asset/obj/cube.obj");
    Vector<Mesh> meshes = load_mesh("E:/code/github/rasterization/asset/obj/viking_room/viking_room.obj");
    Texture tex = load_image("E:/code/github/rasterization/asset/obj/viking_room/viking_room.png");

    RasterizerDescriptor descriptor{
        .rasterizer_state{
            .flip_y = 1,
            .early_z = 1,
            .reverse_z = 0,
        },
        .width = Width,
        .height = Height,
        .mesh = meshes[0],
        .vertexing = [&](Vertex const& in, VertexingBuiltinIn const&, PixelIn& out) -> vec4
        {
            vec4 pos = vec4(in.position.x(), in.position.y(), in.position.z(), 1.0);
            pos = view * model * pos;
            out.pos = vec3(pos.x(), pos.y(), pos.z());
            out.uv = in.tex_coord;
            out.normal = in.normal;
            out.tangent = in.tangent;

            return projection * pos;
        },
        .shading = [&](PixelIn const& in, ShadingBuiltinOut&) -> vec4
        {
            Pixel pixel = bilinear_lod(tex, in.uv);
            return pixel;
        }
    };

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer("rasterizer", Width, Height, 0, &window, &renderer);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, Width, Height);

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
            default:
                break;
            }
        }

        model = translate(vec3(0, 0.5, -2)) * rotate(vec3(0, 1, 0), 90 * SDL_GetTicks() / 1000) * rotate(vec3(01, 0, 0), 90);
        Texture frame = rasterize(descriptor);

        Pixel* pixels = nullptr;
        int pitch;
        SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
        memcpy(pixels, frame.pixels.data(), sizeof(Pixel) * Width * Height);
        SDL_UnlockTexture(texture);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
        SDL_RenderClear(renderer);

        SDL_FRect rect{
            .x = 0,
            .y = 0,
            .w = static_cast<float>(Width),
            .h = static_cast<float>(Height),
        };
        SDL_RenderTexture(renderer, texture, nullptr, &rect);

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}