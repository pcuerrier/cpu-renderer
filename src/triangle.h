#pragma once

#include "vector.h"

union face_t
{
    int data[3];
    struct
    {
        int a;
        int b;
        int c;
    };
};

struct triangle_t
{
    vec2_t points[3];
};
