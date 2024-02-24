#pragma once
#include "vector.h"
#include "triangle.h"

#include <vector>

struct mesh_t
{
    std::vector<vec3_t> vertices;
    std::vector<face_t> faces;
    vec3_t rotation    = { 0.0f, 0.0f, 0.0f };
    vec3_t scale       = { 1.0f, 1.0f, 1.0f };
    vec3_t translation = { 0.0f, 0.0f, 0.0f };
};

bool create_mesh_from_obj(const char* filepath, mesh_t& out_mesh);
