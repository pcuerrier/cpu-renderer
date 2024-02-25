#pragma once

#include "vector.h"

#include <cstdint>

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
    uint32_t color;
};

struct triangle_t
{
    vec2_t points[3];
    uint32_t color = 0x0;
    float  avg_depth = 0.0f;
};
