#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "display.h"
#include "vector.h"

/*******************************************************************************
 * Macros
*******************************************************************************/
#define UNUSED(x) (void)(x)

static vec3_t camera_pos = { 0.0f, 0.0f, -5.0f };

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
 * Project 3D vector to a 2D point
*******************************************************************************/
vec2_t project(const vec3_t v, const float scale)
{
    // Perspective projection
    /*   E| \
          |  \
          |   \
          |    \
          |     \C
          |     |\
          |     | \
          |_____|__\  Eye
          D     B   A */
    // AB = 1 (arbitrarily)
    // AB \ AD == BC \ DE
    // => Projected coordinate BC = AB*DE \ AD
    vec2_t projected = {
        .x = scale * v.x / v.z,
        .y = scale * v.y / v.z
    };
    return projected;
}

/*******************************************************************************
 * Update Logic
*******************************************************************************/
void update(const vec3_t* points, vec2_t* projected_points, uint32_t num_points)
{
    for (uint32_t i = 0; i < num_points; ++i)
    {
        vec3_t point = points[i];
        // Move away from camera
        point.z -= camera_pos.z;

        projected_points[i] = project(point, 640.0f);
    }
}

#include <stdio.h>
/*******************************************************************************
 * Render Color Buffer
*******************************************************************************/
void render(SDL_API sdl, ColorBuffer& color_buffer, vec2_t* projected_points, uint32_t num_points)
{
    draw_grid(color_buffer, 20, 0xFFE4E6EB);

    for (uint32_t i = 0; i < num_points; ++i)
    {
        vec2_t point = projected_points[i];
        draw_rect(
            color_buffer,
            point.x + (color_buffer.width / 2),
            point.y + (color_buffer.height / 2),
            4,
            4,
            0xFFFFFF00
        );
    }

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

    const uint32_t NB_POINTS = 9 * 9 * 9;
    vec3_t points[NB_POINTS];
    uint32_t num_points = 0;
    for (float z = -1.0f; z <= 1.0f; z += 0.25f)
    {
        for (float y = -1.0f; y <= 1.0f; y += 0.25f)
        {
            for (float x = -1.0f; x <= 1.0f; x += 0.25f)
            {
                points[num_points++] = {x, y, z};
            }
        }
    }

    // Main Loop
    vec2_t projected_points[NB_POINTS];
    while (sdl.is_running)
    {
        process_input(sdl, color_buffer);
        update(points, projected_points, NB_POINTS);
        render(sdl, color_buffer, projected_points, NB_POINTS);
    }

    // Free resources
    destroy_color_buffer(color_buffer);
    destroy_window(sdl);
    SDL_Quit();

    return 0;
}
