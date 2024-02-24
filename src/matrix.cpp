#include "matrix.h"

#include <cmath>

mat4_t mat4_identity()
{
    return {{
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }};
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    return {{
        {   sx, 0.0f, 0.0f, 0.0f },
        { 0.0f,   sy, 0.0f, 0.0f },
        { 0.0f, 0.0f,   sz, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }};
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
    return {{
        { 1.0f, 0.0f, 0.0f,   tx },
        { 0.0f, 1.0f, 0.0f,   ty },
        { 0.0f, 0.0f, 1.0f,   tz },
        { 0.0f, 0.0f, 0.0f, 1.0f },
    }};
}

mat4_t mat4_make_rotation_x(float rx)
{
    float s = sin(rx);
    float c = cos(rx);
    return {{
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f,    c,   -s, 0.0f },
        { 0.0f,    s,    c, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }};
}

mat4_t mat4_make_rotation_y(float ry)
{
    float s = sin(ry);
    float c = cos(ry);
    return {{
        {    c, 0.0f,    s, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        {   -s, 0.0f,    c, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }};
}

mat4_t mat4_make_rotation_z(float rz)
{
    float s = sin(rz);
    float c = cos(rz);
    return {{
        {    c,   -s, 0.0f, 0.0f },
        {    s,    c, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }};
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar)
{
    float f = 1 / tan(fov / 2.0f);
    float l = zfar / (zfar - znear);
    return {{
        { aspect * f, 0.0f, 0.0f,       0.0f },
        {       0.0f,    f, 0.0f,       0.0f },
        {       0.0f, 0.0f,    l, -l * znear },
        {       0.0f, 0.0f, 1.0f,       0.0f }
    }};
}

vec4_t mat4_mul_vec4_project(const mat4_t& mat, const vec4_t& vec)
{
    vec4_t result = mat.mul_vec4(vec);

    if (result.w != 0.0f)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

vec4_t mat4_t::mul_vec4(const vec4_t& v) const
{
    return {
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
        m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
    };
}

mat4_t mat4_t::mul_mat4(const mat4_t& n) const
{
    return {{
        // First Row
        { m[0][0] * n.m[0][0] + m[0][1] * n.m[1][0] + m[0][2] * n.m[2][0] + m[0][3] * n.m[3][0],
          m[0][0] * n.m[0][1] + m[0][1] * n.m[1][1] + m[0][2] * n.m[2][1] + m[0][3] * n.m[3][1],
          m[0][0] * n.m[0][2] + m[0][1] * n.m[1][2] + m[0][2] * n.m[2][2] + m[0][3] * n.m[3][2],
          m[0][0] * n.m[0][3] + m[0][1] * n.m[1][3] + m[0][2] * n.m[2][3] + m[0][3] * n.m[3][3] },
        // Second Row
        { m[1][0] * n.m[0][0] + m[1][1] * n.m[1][0] + m[1][2] * n.m[2][0] + m[1][3] * n.m[3][0],
          m[1][0] * n.m[0][1] + m[1][1] * n.m[1][1] + m[1][2] * n.m[2][1] + m[1][3] * n.m[3][1],
          m[1][0] * n.m[0][2] + m[1][1] * n.m[1][2] + m[1][2] * n.m[2][2] + m[1][3] * n.m[3][2],
          m[1][0] * n.m[0][3] + m[1][1] * n.m[1][3] + m[1][2] * n.m[2][3] + m[1][3] * n.m[3][3] },
        // Third Row
        { m[2][0] * n.m[0][0] + m[2][1] * n.m[1][0] + m[2][2] * n.m[2][0] + m[2][3] * n.m[3][0],
          m[2][0] * n.m[0][1] + m[2][1] * n.m[1][1] + m[2][2] * n.m[2][1] + m[2][3] * n.m[3][1],
          m[2][0] * n.m[0][2] + m[2][1] * n.m[1][2] + m[2][2] * n.m[2][2] + m[2][3] * n.m[3][2],
          m[2][0] * n.m[0][3] + m[2][1] * n.m[1][3] + m[2][2] * n.m[2][3] + m[2][3] * n.m[3][3] },
        // Fourth Row
        { m[3][0] * n.m[0][0] + m[3][1] * n.m[1][0] + m[3][2] * n.m[2][0] + m[3][3] * n.m[3][0],
          m[3][0] * n.m[0][1] + m[3][1] * n.m[1][1] + m[3][2] * n.m[2][1] + m[3][3] * n.m[3][1],
          m[3][0] * n.m[0][2] + m[3][1] * n.m[1][2] + m[3][2] * n.m[2][2] + m[3][3] * n.m[3][2],
          m[3][0] * n.m[0][3] + m[3][1] * n.m[1][3] + m[3][2] * n.m[2][3] + m[3][3] * n.m[3][3] },
    }};
}