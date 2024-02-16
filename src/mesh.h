#pragma once
#include "vector.h"
#include "triangle.h"

#include <vector>

struct mesh_t
{
    std::vector<vec3_t> vertices;
    std::vector<face_t> faces;
    vec3_t rotation;
};

bool create_mesh_from_obj(const char* filepath, mesh_t& out_mesh);
