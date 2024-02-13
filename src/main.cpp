#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "display.h"

/*******************************************************************************
 * Macros
*******************************************************************************/
#define UNUSED(x) (void)(x)

/*******************************************************************************
 * Process Input & Events
*******************************************************************************/
void process_input(SDL_API& sdl, ColorBuffer& color_buffer)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                sdl.is_running = false;
            } break;

            case SDL_EVENT_KEY_DOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    sdl.is_running = false;
                }
                else if (event.key.keysym.sym == SDLK_f)
                {
                    sdl.fullscreen = !sdl.fullscreen;
                    SDL_SetWindowFullscreen(sdl.window, sdl.fullscreen);
                }
            } break;

            case SDL_EVENT_WINDOW_RESIZED:
            {
                int width = event.window.data1;
                int height = event.window.data2;
                resize_color_buffer(sdl, color_buffer, width, height);
            } break;
        }
    }
}

/*******************************************************************************
 * Update Logic
*******************************************************************************/
void update(void)
{
}

/*******************************************************************************
 * Render Color Buffer
*******************************************************************************/
void render(SDL_API sdl, ColorBuffer& color_buffer)
{
    SDL_SetRenderDrawColor(sdl.renderer, 255, 0, 0, 255);
    SDL_RenderClear(sdl.renderer);

    draw_grid(color_buffer, 20, 0xFFE4E6EB);

    //draw_pixel(color_buffer, 20, 20, 0xFFFF00FF);

    uint32_t x = 56;
    uint32_t y = 127;
    uint32_t width = 350;
    uint32_t height = 125;
    uint32_t color = 0xFFFC2AAA;
    draw_rect(color_buffer, x, y, width, height, color);

    // AA RR GG BB
    SDL_UpdateTexture(
        color_buffer.texture,
        NULL,
        color_buffer.memory,
        sizeof(uint32_t) * color_buffer.width
    );
    //SDL_FRect rect = {};
    //rect.w = (float)color_buffer.width;
    //rect.h = (float)color_buffer.height;
    SDL_RenderTexture(sdl.renderer, color_buffer.texture, NULL, NULL);
    clear_color_buffer(color_buffer, 0xFF18191A);

    SDL_RenderPresent(sdl.renderer);
}

/*******************************************************************************
 * Main Entry
*******************************************************************************/
int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    SDL_API sdl = initialize_window();
    if (!sdl.window || !sdl.renderer)
    {
        return 1;
    }

    ColorBuffer color_buffer = {};
    create_color_buffer(sdl, color_buffer);
    clear_color_buffer(color_buffer, 0xFF18191A);

    // Main Loop
    while (sdl.is_running)
    {
        process_input(sdl, color_buffer);
        update();
        render(sdl, color_buffer);
    }

    // Free resources
    destroy_color_buffer(color_buffer);
    destroy_window(sdl);
    SDL_Quit();

    return 0;
}
