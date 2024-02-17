#pragma once

#include "vector.h"

struct face_t
{
    union
    {
        struct
        {
            int a;
            int b;
            int c;
        };
        int data[3];
    };
};

struct triangle_t
{
    vec2_t points[3];
};
