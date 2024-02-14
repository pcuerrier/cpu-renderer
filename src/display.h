#pragma once
#include <SDL3/SDL.h>

/*******************************************************************************
 * Structures
*******************************************************************************/
struct SDL_API
{
    SDL_Window*   window     = nullptr;
    SDL_Renderer* renderer   = nullptr;
    bool          is_running = false;
    bool          fullscreen = true;
};

struct ColorBuffer
{
    uint32_t*    memory   = nullptr;
    SDL_Texture* texture  = nullptr;
    uint32_t     width    = 0;
    uint32_t     height   = 0;
};

/*******************************************************************************
 * SDL related Functions
*******************************************************************************/
SDL_API initialize_window(void);
void    destroy_window(SDL_API& sdl);

/*******************************************************************************
 * Color Buffer related Functions
*******************************************************************************/
void clear_color_buffer(ColorBuffer& color_buffer, uint32_t color);
void create_color_buffer(SDL_API sdl, ColorBuffer& color_buffer);
void resize_color_buffer(SDL_API sdl, ColorBuffer& color_buffer, uint32_t width, uint32_t height);
void destroy_color_buffer(ColorBuffer& color_buffer);

/*******************************************************************************
 * Draw Functions
*******************************************************************************/
void draw_pixel(ColorBuffer& color_buffer, int x, int y, uint32_t color);
void draw_grid(ColorBuffer& color_buffer, uint32_t size, uint32_t color);
void draw_rect(ColorBuffer& color_buffer, int x, int y, uint32_t width, uint32_t height, uint32_t color);
