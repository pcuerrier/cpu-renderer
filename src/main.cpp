#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdio>
#include <cstdlib>

/*******************************************************************************
 * Macros
*******************************************************************************/
#define UNUSED(x) (void)(x)

/*******************************************************************************
 * Constants
*******************************************************************************/
static const uint32_t WINDOW_WIDTH  = 800;
static const uint32_t WINDOW_HEIGHT = 600;

/*******************************************************************************
 * Globals
*******************************************************************************/
static bool g_is_running = true;

/*******************************************************************************
 * Structures
*******************************************************************************/
struct SDL_API
{
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
};

struct ColorBuffer
{
    uint32_t*    memory  = nullptr;
    SDL_Texture* texture = nullptr;
};

/*******************************************************************************
 * Initialize SDL Window & Renderer
*******************************************************************************/
SDL_API initialize_window(void)
{
    SDL_API result = {}; 
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        // Error when initiliazing SDL
        fprintf(stderr, "Error when initializing SDL : %s\n", SDL_GetError());
        return result;
    }

    result.window = SDL_CreateWindow("CPU-Renderer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!result.window)
    {
        fprintf(stderr, "Error when creating the window : %s\n", SDL_GetError());
        return result;
    }

    result.renderer = SDL_CreateRenderer(result.window, 0, 0);
    if (!result.renderer)
    {
        fprintf(stderr, "Error when creating the renderer : %s\n", SDL_GetError());
        return result;
    }

    return result;
}

/*******************************************************************************
 * Clear Color Buffer
*******************************************************************************/
void clear_color_buffer(uint32_t* color_buffer, uint32_t color)
{
    // Set every pixel to 'color'
    for (uint32_t row = 0; row < WINDOW_HEIGHT; ++row)
    {
        for (uint32_t column = 0; column < WINDOW_WIDTH; ++column)
        {
            color_buffer[WINDOW_WIDTH * row + column] = color;
        }
    }
}

/*******************************************************************************
 * Create Color Buffer
*******************************************************************************/
void create_color_buffer(SDL_API sdl, ColorBuffer* color_buffer)
{
    color_buffer->memory = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    color_buffer->texture = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
}

/*******************************************************************************
 * Process Input & Events
*******************************************************************************/
void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                g_is_running = false;
            } break;

            case SDL_EVENT_KEY_DOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    g_is_running = false;
                }
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
void render(SDL_API sdl, ColorBuffer* color_buffer)
{
    SDL_SetRenderDrawColor(sdl.renderer, 255, 0, 0, 255);
    SDL_RenderClear(sdl.renderer);

    // AA RR GG BB
    SDL_UpdateTexture(
        color_buffer->texture,
        NULL,
        color_buffer->memory,
        sizeof(uint32_t) * WINDOW_WIDTH
    );
    SDL_RenderTexture(sdl.renderer, color_buffer->texture, NULL, NULL);
    clear_color_buffer(color_buffer->memory, 0xFFAAFF44);

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
    create_color_buffer(sdl, &color_buffer);

    // Main Loop
    while (g_is_running)
    {
        process_input();
        update();
        render(sdl, &color_buffer);
    }

    // Free resources
    SDL_DestroyTexture(color_buffer.texture);
    free(color_buffer.memory);
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();

    return 0;
}
