#pragma once

#include "vector.h"

struct face_t
{
    int a = 0;
    int b = 0;
    int c = 0;
};

struct triangle_t
{
    vec2_t points[3];
};
