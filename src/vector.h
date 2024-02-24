#pragma once

struct vec2_t;
struct vec3_t;
struct vec4_t;

struct vec2_t
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float data[2];
    };

    float length() const;
    void normalize();

    vec2_t operator + (const vec2_t other) const;
    vec2_t operator - (const vec2_t other) const;
    vec2_t operator * (const float scalar) const;
    vec2_t operator / (const float scalar) const;
};

struct vec3_t
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float data[3];
    };

    float length() const;

    void normalize();

    vec3_t operator + (const vec3_t& other) const;
    vec3_t operator - (const vec3_t& other) const;
    vec3_t operator * (const float scalar) const;
    vec3_t operator / (const float scalar) const;

    float dot_product(const vec3_t& other) const;
    vec3_t cross_product(const vec3_t& other) const;

    vec3_t rotate_x(const float angle) const;
    vec3_t rotate_y(const float angle) const;
    vec3_t rotate_z(const float angle) const;

    vec4_t to_vec4() const;
};

struct vec4_t
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float data[4];
    };

    vec3_t to_vec3() const;
};
