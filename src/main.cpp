#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

#include <vector>
#include <cmath>

/*******************************************************************************
 * Macros
*******************************************************************************/
#define UNUSED(x) (void)(x)

/*******************************************************************************
 * Constants
*******************************************************************************/
const int FPS = 30;
const float FRAME_TARGET_TIME = (1000.0f) / FPS;
const float SCALE = 640.0f;

/*******************************************************************************
 * Globals
*******************************************************************************/
static vec3_t camera_pos = { 0.0f, 0.0f, -5.0f };
static std::vector<triangle_t> triangles;

static mesh_t mesh;

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
vec2_t project(const vec3_t v)
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
        .x = SCALE * v.x / v.z,
        .y = SCALE * v.y / v.z
    };
    return projected;
}

/*******************************************************************************
 * Update Logic
*******************************************************************************/
void update(uint32_t window_width, uint32_t window_height)
{
    mesh.rotation.x += 0.01f;
    mesh.rotation.y += 0.01f;
    mesh.rotation.z += 0.01f;

    for (size_t i = 0; i < mesh.faces.size(); ++i)
    {
        face_t mesh_face = mesh.faces[i];
        vec3_t face_vertices[3] = {
            mesh.vertices[mesh_face.a],
            mesh.vertices[mesh_face.b],
            mesh.vertices[mesh_face.c]
        };
        triangle_t projected_triangle = {};
        for (int j = 0; j < 3; ++j)
        {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_x(transformed_vertex,
                mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex,
                mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex,
                mesh.rotation.z);

            // Translate the points aways from the camera
            transformed_vertex.z -= camera_pos.z;

            vec2_t projected_point = project(transformed_vertex);

            // Scale and translate the points to the middle of the screen
            projected_point.x += window_width / 2;
            projected_point.y += window_height / 2;

            projected_triangle.points[j] = projected_point;   
        }
        triangles.push_back(projected_triangle);
    }
}

#include <stdio.h>
/*******************************************************************************
 * Render Color Buffer
*******************************************************************************/
void render(SDL_API sdl, ColorBuffer& color_buffer)
{
    draw_grid(color_buffer, 20, 0xFFE4E6EB);

    for (uint32_t i = 0; i < triangles.size(); ++i)
    {
        triangle_t triangle = triangles[i];
        draw_triangle(
            color_buffer,
            (int)round(triangle.points[0].x),
            (int)round(triangle.points[0].y),
            (int)round(triangle.points[1].x),
            (int)round(triangle.points[1].y),
            (int)round(triangle.points[2].x),
            (int)round(triangle.points[2].y),
            0xFFFFFF00
        );
    }
    triangles.clear();

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

    mesh.vertices.push_back({ .x = -1, .y = -1, .z = -1 }); // 0
    mesh.vertices.push_back({ .x = -1, .y =  1, .z = -1 }); // 1
    mesh.vertices.push_back({ .x =  1, .y =  1, .z = -1 }); // 2
    mesh.vertices.push_back({ .x =  1, .y = -1, .z = -1 }); // 3
    mesh.vertices.push_back({ .x =  1, .y =  1, .z =  1 }); // 4
    mesh.vertices.push_back({ .x =  1, .y = -1, .z =  1 }); // 5
    mesh.vertices.push_back({ .x = -1, .y =  1, .z =  1 }); // 6
    mesh.vertices.push_back({ .x = -1, .y = -1, .z =  1 }); // 7

    // front
    mesh.faces.push_back({ .a = 0, .b = 1, .c = 2 });
    mesh.faces.push_back({ .a = 0, .b = 0, .c = 3 });
    // right
    mesh.faces.push_back({ .a = 3, .b = 2, .c = 4 });
    mesh.faces.push_back({ .a = 3, .b = 4, .c = 5 });
    // back
    mesh.faces.push_back({ .a = 5, .b = 4, .c = 6 });
    mesh.faces.push_back({ .a = 5, .b = 6, .c = 7 });
    // left
    mesh.faces.push_back({ .a = 7, .b = 6, .c = 1 });
    mesh.faces.push_back({ .a = 7, .b = 1, .c = 0 });
    // top
    mesh.faces.push_back({ .a = 1, .b = 6, .c = 4 });
    mesh.faces.push_back({ .a = 1, .b = 4, .c = 2 });
    // bottom
    mesh.faces.push_back({ .a = 5, .b = 7, .c = 0 });
    mesh.faces.push_back({ .a = 5, .b = 0, .c = 3 });

    // Main Loop
    while (sdl.is_running)
    {
        uint32_t start_frame_ticks = (uint32_t)SDL_GetTicks();
        process_input(sdl, color_buffer);
        update(color_buffer.width, color_buffer.height);
        render(sdl, color_buffer);
        uint32_t elapsed_frame_ticks = (uint32_t)SDL_GetTicks() - start_frame_ticks;
        if ((float)elapsed_frame_ticks <= FRAME_TARGET_TIME)
        {
            SDL_Delay((uint32_t)
                (FRAME_TARGET_TIME - (float)elapsed_frame_ticks));
        }
    }

    // Free resources
    destroy_color_buffer(color_buffer);
    destroy_window(sdl);
    SDL_Quit();

    return 0;
}
