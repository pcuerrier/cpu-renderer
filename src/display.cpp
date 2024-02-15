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

    result.window = SDL_CreateWindow("CPU-Renderer", 800, 600,
                                     SDL_WINDOW_RESIZABLE);
    if (!result.window)
    {
        fprintf(stderr, "Error when creating the window : %s\n",
                SDL_GetError());
        return result;
    }
    result.fullscreen = false;

    result.renderer = SDL_CreateRenderer(result.window, 0, 0);
    if (!result.renderer)
    {
        fprintf(stderr, "Error when creating the renderer : %s\n",
                SDL_GetError());
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
void resize_color_buffer(SDL_API sdl, ColorBuffer& color_buffer, uint32_t width,
                         uint32_t height)
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
void draw_pixel(ColorBuffer& color_buffer, int x, int y, uint32_t color)
{
    if (x >= 0 && x < (int)color_buffer.width &&
        y >= 0 && y < (int)color_buffer.height)
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
            for (uint32_t col = 0; col < color_buffer.width; col += size)
            {
                color_buffer.memory[color_buffer.width * row + col] = color;
            }
        }
    }
}

/*******************************************************************************
 * Draw Rectangle
*******************************************************************************/
void draw_rect(ColorBuffer& color_buffer, int x, int y, uint32_t width,
               uint32_t height, uint32_t color)
{
    for (int row = 0; row < (int)height; ++row)
    {
        for (int column = 0; column < (int)width; ++column)
        {
            int current_x = x + column;
            int current_y = y + row;
            draw_pixel(color_buffer, current_x, current_y, color);
        }
    }
}

/*******************************************************************************
 * Draw Line
*******************************************************************************/
void draw_line(ColorBuffer& color_buffer, int x0, int y0, int x1, int y1,
               uint32_t color)
{
    // Based on DDA
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_length = abs(delta_x) >= abs(delta_y) ?
                      abs(delta_x) :
                      abs(delta_y);

    // How much incerement x and y each step
    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = (float)x0;
    float current_y = (float)y0;

    for (int i = 0; i < side_length; ++i)
    {
        draw_pixel(color_buffer, (int)round(current_x),
                   (int)round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(ColorBuffer& color_buffer,
                   int x0, int y0,
                   int x1, int y1,
                   int x2, int y2,
                   uint32_t color)
{
    draw_line(color_buffer, x0, y0, x1, y1, color);
    draw_line(color_buffer, x1, y1, x2, y2, color);
    draw_line(color_buffer, x2, y2, x0, y0, color);
}
