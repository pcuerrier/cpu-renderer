#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

#include <vector>
#include <cmath>

#include <stdio.h>
#include <limits.h>

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
        SCALE * v.x / v.z,
        SCALE * v.y / v.z
    };
    return projected;
}

/*******************************************************************************
 * Update Logic
*******************************************************************************/
void update(uint32_t window_width, uint32_t window_height)
{
    mesh.rotation.x += 0.02f;
    mesh.rotation.y += 0.00f;
    mesh.rotation.z += 0.00f;

    for (size_t i = 0; i < mesh.faces.size(); ++i)
    {
        face_t mesh_face = mesh.faces[i];
        vec3_t face_vertices[3] = {
            mesh.vertices[mesh_face.a - 1],
            mesh.vertices[mesh_face.b - 1],
            mesh.vertices[mesh_face.c - 1]
        };

        vec3_t transformed_vertices[3];
        for (int j = 0; j < 3; ++j)
        {
            vec3_t& transformed_vertex = face_vertices[j];
            transformed_vertex = transformed_vertex.rotate_x(mesh.rotation.x);
            transformed_vertex = transformed_vertex.rotate_y(mesh.rotation.y);
            transformed_vertex = transformed_vertex.rotate_z(mesh.rotation.z);

            // Translate the points aways from the camera
            transformed_vertex.z -= camera_pos.z;
            transformed_vertices[j] = transformed_vertex;
        }

        // Back-face culling
        vec3_t& vertex_a = transformed_vertices[0]; /*   A   */
        vec3_t& vertex_b = transformed_vertices[1]; /*  / \  */
        vec3_t& vertex_c = transformed_vertices[2]; /* C---B */

        vec3_t vector_ab = vertex_b - vertex_a;
        vec3_t vector_ac = vertex_c - vertex_a;
        vec3_t normal = vector_ab.cross_product(vector_ac);
        normal.normalize();
        vec3_t camera_ray = camera_pos - vertex_a;

        float dot_normal_camera = normal.dot_product(camera_ray);
        if (dot_normal_camera < 0) { continue; }

        triangle_t projected_triangle = {};
        for (int j = 0; j < 3; ++j)
        {
            vec2_t projected_point = project(transformed_vertices[j]);

            // Scale and translate the points to the middle of the screen
            projected_point.x += window_width / 2;
            projected_point.y += window_height / 2;

            projected_triangle.points[j] = projected_point;   
        }
        triangles.push_back(projected_triangle);
    }
}

/*******************************************************************************
 * Render Color Buffer
*******************************************************************************/
void render(SDL_API sdl, ColorBuffer& color_buffer)
{
    draw_grid(color_buffer, 20, 0xFFE4E6EB);

    uint32_t colors[] = {
        0xFFFF0000, //red
        0xFFFF8000, //orange
        0xFFFFFF00, //yellow
        0xFF80FF00, //chartreuse
        0xFF00FF00, //green
        0xFF00FF80, //spring green
        0xFF00FFFF, //cyan
        0xFF0080FF, //dodger blue
        0xFF0000FF, //blue
        0xFF8000FF, //purple
        0xFFFF00FF, //violet
        0xFFFF0080  //magenta
    };
    for (uint32_t i = 0; i < triangles.size(); ++i)
    {
        triangle_t triangle = triangles[i];
        draw_filled_triangle(
            color_buffer,
            (int)round(triangle.points[0].x),
            (int)round(triangle.points[0].y),
            (int)round(triangle.points[1].x),
            (int)round(triangle.points[1].y),
            (int)round(triangle.points[2].x),
            (int)round(triangle.points[2].y),
            colors[i]
            //0xFFFFFF00
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

    //
    //char cwd[PATH_MAX];
    //getcwd(cwd, sizeof(cwd));
    //fprintf(stdout, "CWD : %s\n", cwd);
#ifdef WIN32
    create_mesh_from_obj("../../../assets/f22.obj", mesh);
#else
    create_mesh_from_obj("../../assets/cube.obj", mesh);
#endif

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
