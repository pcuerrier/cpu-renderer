#pragma once

#include "vector.h"

struct mat4_t
{
    float m[4][4];

    vec4_t mul_vec4(const vec4_t& v) const;
    mat4_t mul_mat4(const mat4_t& n) const;
};

mat4_t mat4_identity();
mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation_x(float rx);
mat4_t mat4_make_rotation_y(float ry);
mat4_t mat4_make_rotation_z(float rz);
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mul_vec4_project(const mat4_t& mat, const vec4_t& vec);