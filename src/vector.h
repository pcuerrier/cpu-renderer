#pragma once


struct vec2_t
{
    float x = 0;
    float y = 0;
};

union vec3_t
{
    struct
    {
        float x;
        float y;
        float z;
    };
    float data[3];
} vec3;

vec3_t vec3_rotate_x(const vec3_t v, float angle);
vec3_t vec3_rotate_y(const vec3_t v, float angle);
vec3_t vec3_rotate_z(const vec3_t v, float angle);