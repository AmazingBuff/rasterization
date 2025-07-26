//
// Created by AmazingBuff on 25-7-25.
//

#include <rasterizer.h>
#include <chrono>
#include <SDL3/SDL.h>

using namespace Amazing::Rasterizer;

constexpr static uint32_t Width = 960;
constexpr static uint32_t Height = 540;

int main()
{
    Mesh mesh{
        .vertices = {{{0.0f, 0.5f, 0.0f}, {0.5f, 0.5f}},
                     {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
                     {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}},
    };

    RasterizerDescriptor descriptor{
        .rasterizer_state{
            .flip_y = 1,
            .early_z = 0
        },
        .width = Width,
        .height = Height,
        .mesh{
            .vertices = {{{0.0f, 0.5f, 0.0f}, {0.5f, 0.5f}},
                     {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
                     {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}},
        },
        .vertexing = [](Vertex const& in, PixelIn& out) -> vec4
        {
            out.pos = in.position;
            out.uv = in.tex_coord;
            out.normal = in.normal;
            out.tangent = in.tangent;
            return {in.position.x(), in.position.y(), in.position.z(), 1.0};
        },
        .shading = [](PixelIn const& in) -> vec4
        {
            return {in.pos.x(), in.pos.y(), in.pos.z(), 1.0};
        }
    };

    auto start = std::chrono::high_resolution_clock::now();

    PixelFrame frame = rasterize(descriptor);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("examples/renderer/textures", Width, Height, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(Width, Height, SDL_PIXELFORMAT_RGBA32, frame.pixels.data(), Width * sizeof(Pixel));
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

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