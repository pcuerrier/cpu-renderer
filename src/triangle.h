#pragma once

#include "vector.h"

struct face_t
{
    union
    {
        int data[3] = { 0, 0, 0 };
        struct indices
        {
            int a = 0;
            int b = 0;
            int c = 0;
        } indices;
    };
};

struct triangle_t
{
    vec2_t points[3];
};
