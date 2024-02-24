#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

#include "display.h"
#include "vector.h"
#include "matrix.h"
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
static mat4_t projection_matrix = mat4_identity();
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
                else if (event.key.keysym.sym == SDLK_c)
                {
                    sdl.culling = true;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    sdl.culling = false;
                }
                else if (event.key.keysym.sym == SDLK_1)
                {
                    sdl.render_mode = RENDER_MODE::WIREFRAME_DOTS;
                }
                else if (event.key.keysym.sym == SDLK_2)
                {
                    sdl.render_mode = RENDER_MODE::WIREFRAME_LINES;
                }
                else if (event.key.keysym.sym == SDLK_3)
                {
                    sdl.render_mode = RENDER_MODE::FILLED_TRIANGLES;
                }
                else if (event.key.keysym.sym == SDLK_4)
                {
                    sdl.render_mode = RENDER_MODE::FILLED_TRIANGLES_AND_WIREFRAME;
                }
            } break;

            case SDL_EVENT_WINDOW_RESIZED:
            {
                int width = event.window.data1;
                int height = event.window.data2;
                resize_color_buffer(sdl, color_buffer, width, height);

                // Remake perspective matrix
                float fov = M_PI / 3.0f; // 60 deg
                float aspect = (float)color_buffer.height / color_buffer.width;
                float znear = 0.1f;
                float zfar = 100.0f;
                projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);
            } break;
        }
    }
}

/*******************************************************************************
 * Project 3D vector to a 2D point
*******************************************************************************/
/*vec2_t project(const vec3_t v)
{*/
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
/*    vec2_t projected = {
        SCALE * v.x / v.z,
        SCALE * v.y / v.z
    };
    return projected;
}*/

/*******************************************************************************
 * Update Logic
*******************************************************************************/
void update(const SDL_API& sdl, uint32_t window_width, uint32_t window_height)
{
    mesh.rotation.x += 0.01f;
    //mesh.rotation.y += 0.01f;
    //mesh.rotation.z += 0.01f;

    //mesh.scale.x += 0.002f;
    //mesh.scale.y += 0.001f;
    //mesh.translation.x += 0.01f;

    // Translate the points aways from the camera
    mesh.translation.z = -camera_pos.z;

    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t rotation_x_matrix = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_y_matrix = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_z_matrix = mat4_make_rotation_z(mesh.rotation.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t world_matrix = mat4_identity();
    world_matrix = scale_matrix.mul_mat4(world_matrix);
    world_matrix = rotation_x_matrix.mul_mat4(world_matrix);
    world_matrix = rotation_y_matrix.mul_mat4(world_matrix);
    world_matrix = rotation_z_matrix.mul_mat4(world_matrix);
    world_matrix = translation_matrix.mul_mat4(world_matrix);
    for (size_t i = 0; i < mesh.faces.size(); ++i)
    {
        face_t mesh_face = mesh.faces[i];
        const vec3_t face_vertices[3] = {
            mesh.vertices[mesh_face.a - 1],
            mesh.vertices[mesh_face.b - 1],
            mesh.vertices[mesh_face.c - 1]
        };

        vec4_t transformed_vertices[3];
        for (int j = 0; j < 3; ++j)
        {
            vec4_t transformed_vertex = face_vertices[j].to_vec4();
            //transformed_vertex = transformed_vertex.rotate_x(mesh.rotation.x);
            //transformed_vertex = transformed_vertex.rotate_y(mesh.rotation.y);
            //transformed_vertex = transformed_vertex.rotate_z(mesh.rotation.z);

            transformed_vertex = world_matrix.mul_vec4(transformed_vertex);
            transformed_vertex.z -= camera_pos.z;
            transformed_vertices[j] = transformed_vertex;
        }

        // Back-face culling
        if (sdl.culling)
        {
            vec3_t vertex_a = transformed_vertices[0].to_vec3(); /*   A   */
            vec3_t vertex_b = transformed_vertices[1].to_vec3(); /*  / \  */
            vec3_t vertex_c = transformed_vertices[2].to_vec3(); /* C---B */

            vec3_t vector_ab = vertex_b - vertex_a;
            vec3_t vector_ac = vertex_c - vertex_a;
            vec3_t normal = vector_ab.cross_product(vector_ac);
            normal.normalize();
            vec3_t camera_ray = camera_pos - vertex_a;

            float dot_normal_camera = normal.dot_product(camera_ray);
            if (dot_normal_camera <= 0) { continue; }
        }

        triangle_t projected_triangle = {};
        float cumulative_z = 0.0f;
        for (int j = 0; j < 3; ++j)
        {
            //vec2_t projected_point = project(transformed_vertices[j]);
            vec4_t projected_point = mat4_mul_vec4_project(projection_matrix, transformed_vertices[j]);

            // Scale into the view
            projected_point.x *= window_width / 2.0f;
            projected_point.y *= window_height / 2.0f;

            // Translate the points to the middle of the screen
            projected_point.x += window_width / 2.0f;
            projected_point.y += window_height / 2.0f;

            projected_triangle.points[j].x = projected_point.x;
            projected_triangle.points[j].y = projected_point.y;
            cumulative_z += projected_point.z;
        }
        projected_triangle.avg_depth = cumulative_z / 3.0f;
        triangles.push_back(projected_triangle);
    }

    // TODO: Sort triangles based on the avg_depth
    for (size_t i = 0; i < triangles.size(); ++i)
    {
        for (size_t j = i + 1; j < triangles.size(); ++j)
        {
            if (triangles[i].avg_depth < triangles[j].avg_depth)
            {
                triangle_t temp = triangles[i];
                triangles[i] = triangles[j];
                triangles[j] = temp;
            }
        }
    }
}

/*******************************************************************************
 * Render Color Buffer
*******************************************************************************/
void render(const SDL_API& sdl, ColorBuffer& color_buffer)
{
    draw_grid(color_buffer, 20, 0xFFE4E6EB);

    /*uint32_t colors[] = {
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
    };*/

    for (uint32_t i = 0; i < triangles.size(); ++i)
    {
        triangle_t triangle = triangles[i];
        if (sdl.render_mode != RENDER_MODE::FILLED_TRIANGLES)
        {
            // Draw wireframe
            draw_line(color_buffer, triangle.points[0].x, triangle.points[0].y,
                      triangle.points[1].x, triangle.points[1].y, 0xFF0000FF);
            draw_line(color_buffer, triangle.points[1].x, triangle.points[1].y,
                      triangle.points[2].x, triangle.points[2].y, 0xFF0000FF);
            draw_line(color_buffer, triangle.points[2].x, triangle.points[2].y,
                      triangle.points[0].x, triangle.points[0].y, 0xFF0000FF);
        }
        if (sdl.render_mode == RENDER_MODE::WIREFRAME_DOTS)
        {
            // Draw vertex dots
            draw_rect(color_buffer, triangle.points[0].x, triangle.points[0].y,
                      3, 3, 0xFFFF0000);
            draw_rect(color_buffer, triangle.points[1].x, triangle.points[1].y,
                      3, 3, 0xFFFF0000);
            draw_rect(color_buffer, triangle.points[2].x, triangle.points[2].y,
                      3, 3, 0xFFFF0000);
        }
        else if (sdl.render_mode == RENDER_MODE::FILLED_TRIANGLES ||
                 sdl.render_mode == RENDER_MODE::FILLED_TRIANGLES_AND_WIREFRAME)
        {
            draw_filled_triangle(
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

    float fov = M_PI / 3.0f; // 60 deg
    float aspect = (float)color_buffer.height / color_buffer.width;
    float znear = 0.1f;
    float zfar = 100.0f;
    projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

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
        update(sdl, color_buffer.width, color_buffer.height);
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
