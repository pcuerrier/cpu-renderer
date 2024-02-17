#include "vector.h"

#include <cmath>

float vec2_t::length() const
{
    return sqrtf(x * x + y * y);
}

void vec2_t::normalize()
{
    float inverse = 1.0f / length();
    x *= inverse;
    y *= inverse;
}

vec2_t vec2_t::operator + (const vec2_t other) const
{
    return {
        x + other.x,
        y + other.y
    };
}

vec2_t vec2_t::operator - (const vec2_t other) const
{
    return {
        x - other.x,
        y - other.y
    };
}

vec2_t vec2_t::operator * (const float scalar) const
{
    return {
        x * scalar,
        y * scalar
    };
}

vec2_t vec2_t::operator / (const float scalar) const
{
    float inverse = 1.0f / scalar;
    return {
        x * inverse,
        y * inverse
    };
}

float vec3_t::length() const
{
    return sqrtf(x * x + y * y + z * z);
}

void vec3_t::normalize()
{
    float inverse = 1.0f / length();
    x *= inverse;
    y *= inverse;
    z *= inverse;
}

vec3_t vec3_t::operator + (const vec3_t& other) const
{
    return {
        x + other.x,
        y + other.y,
        z + other.z
    };
}

vec3_t vec3_t::operator - (const vec3_t& other) const
{
    return {
        x - other.x,
        y - other.y,
        z - other.z
    };
}

vec3_t vec3_t::operator * (const float scalar) const
{
    return {
        x * scalar,
        y * scalar,
        z * scalar
    };
}

vec3_t vec3_t::operator / (const float scalar) const
{
    float inverse = 1.0f / scalar;
    return {
        x * inverse,
        y * inverse,
        z * inverse
    };
}

float vec3_t::dot_product(const vec3_t& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

vec3_t vec3_t::cross_product(const vec3_t& other) const
{
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

vec3_t vec3_t::rotate_x(float angle) const
{
    return {
        x,
        y * cosf(angle) - z * sinf(angle),
        y * sinf(angle) + z * cosf(angle)
    };
}

vec3_t vec3_t::rotate_y(float angle) const
{
    return {
        x * cosf(angle) - z * sinf(angle),
        y,
        x * sinf(angle) + z * cosf(angle)
    };
}

vec3_t vec3_t::rotate_z(float angle) const
{
    return {
        x * cosf(angle) - y * sinf(angle),
        x * sinf(angle) + y * cosf(angle),
        z
    };
}