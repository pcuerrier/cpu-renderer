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

