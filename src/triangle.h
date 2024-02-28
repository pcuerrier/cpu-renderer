#pragma once

#include "vector.h"
#include "texture.h"

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
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    uint32_t color;
};

struct triangle_t
{
    vec4_t points[3];
    tex2_t texcoord[3];
    uint32_t color = 0x0;
    float  avg_depth = 0.0f;
};

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);
