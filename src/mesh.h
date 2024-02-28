#pragma once
#include "vector.h"
#include "triangle.h"

#include <vector>

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

struct mesh_t
{
    std::vector<vec3_t> vertices;
    std::vector<face_t> faces;
    vec3_t rotation    = { 0.0f, 0.0f, 0.0f };
    vec3_t scale       = { 1.0f, 1.0f, 1.0f };
    vec3_t translation = { 0.0f, 0.0f, 0.0f };
};

bool create_mesh_from_obj(const char* filepath, mesh_t& out_mesh);

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file_data(char* filename);
