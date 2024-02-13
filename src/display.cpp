#include "display.h"

#include <cstdio>
#include <cstdlib>

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

    result.window = SDL_CreateWindow("CPU-Renderer", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!result.window)
    {
        fprintf(stderr, "Error when creating the window : %s\n", SDL_GetError());
        return result;
    }
    result.fullscreen = false;

    result.renderer = SDL_CreateRenderer(result.window, 0, 0);
    if (!result.renderer)
    {
        fprintf(stderr, "Error when creating the renderer : %s\n", SDL_GetError());
        return result;
    }

    result.is_running = true;
    return result;
}

/*******************************************************************************
 * Destroy SDL Window & Renderer
*******************************************************************************/
void destroy_window(SDL_API& sdl)
{
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
}

/*******************************************************************************
 * Clear Color Buffer
*******************************************************************************/
void clear_color_buffer(ColorBuffer& color_buffer, uint32_t color)
{
    // Set every pixel to 'color'
    for (uint32_t row = 0; row < color_buffer.height; ++row)
    {
        for (uint32_t column = 0; column < color_buffer.width; ++column)
        {
            color_buffer.memory[color_buffer.width * row + column] = color;
        }
    }
}

/*******************************************************************************
 * Create Color Buffer
*******************************************************************************/
void create_color_buffer(SDL_API sdl, ColorBuffer& color_buffer)
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(sdl.window, &width, &height);
    color_buffer.memory = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    color_buffer.texture = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
    color_buffer.width = width;
    color_buffer.height = height;
}

/*******************************************************************************
 * Resize Color Buffer
*******************************************************************************/
void resize_color_buffer(SDL_API sdl, ColorBuffer& color_buffer, uint32_t width, uint32_t height)
{
    if (color_buffer.texture)
    {
        SDL_DestroyTexture(color_buffer.texture);
    }
    if (color_buffer.memory)
    {
        free(color_buffer.memory);
    }
    color_buffer.memory = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    color_buffer.texture = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
    color_buffer.width = width;
    color_buffer.height = height;
}

/*******************************************************************************
 * Destroy Color Buffer
*******************************************************************************/
void destroy_color_buffer(ColorBuffer& color_buffer)
{
    color_buffer.width = 0;
    color_buffer.height = 0;
    SDL_DestroyTexture(color_buffer.texture);
    free(color_buffer.memory);
    color_buffer.memory = nullptr;
}

/*******************************************************************************
 * Draw a single pixel
*******************************************************************************/
void draw_pixel(ColorBuffer& color_buffer, uint32_t x, uint32_t y, uint32_t color)
{
    if (x < color_buffer.width && y < color_buffer.height)
    {
        color_buffer.memory[color_buffer.width * y + x] = color;
    }
}

/*******************************************************************************
 * Draw Grid
*******************************************************************************/
void draw_grid(ColorBuffer& color_buffer, uint32_t size, uint32_t color)
{
    if (size > 0)
    {
        for (uint32_t row = 0; row < color_buffer.height; row += size)
        {
            for (uint32_t column = 0; column < color_buffer.width; column += size)
            {
                color_buffer.memory[color_buffer.width * row + column] = color;
            }
        }
    }
}

/*******************************************************************************
 * Draw Rectangle
*******************************************************************************/
void draw_rect(ColorBuffer& color_buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    if ((x + width) <= color_buffer.width && (y + height) <= color_buffer.height)
    {
        for (uint32_t row = y; row < (y + height); ++row)
        {
            for (uint32_t column = x; column < (x + width); ++column)
            {
                color_buffer.memory[color_buffer.width * row + column] = color;
            }
        }
    }
}