#include <stdint.h>
#include <SDL2/SDL.h>

#include "../inc/display.h"

static const uint32_t gb_palette[4] = {
    0xFFFFFFFF,
    0xC0C0C0FF,
    0x606060FF,
    0x000000FF
};

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;

void display_init(DisplayContext* ctx)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT
    );

    ctx->is_running = 1;
}

void display_poll(DisplayContext* ctx)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                display_quit(ctx);
                break;
            default:
                break;
        }
    }
}

void display_quit(DisplayContext* ctx)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(window);
    SDL_Quit();

    ctx->is_running = 0;
}

void display_render(Ppu* ppu)
{
    static uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
        for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
            pixels[y * SCREEN_WIDTH + x] = gb_palette[ppu->framebuffer[y][x]];

    SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}