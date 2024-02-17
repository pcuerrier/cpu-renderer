#include "gtest/gtest.h"
#include "vector.h"

const float PI = 3.141596f;
const float EPSILON = 0.001f;

float deg_to_rad(float deg)
{
    return deg * PI / 180.0f;
}

TEST(Vector, vec3_rotate_x)
{
    vec3_t v = {1, 1, 1};
    vec3_t v1 = v.rotate_x(deg_to_rad(15.0f));
    EXPECT_TRUE(abs(v1.x - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v1.y - 0.707f) <= EPSILON);
    EXPECT_TRUE(abs(v1.z - 1.225f) <= EPSILON);

    vec3_t v2 = v.rotate_x(deg_to_rad(-15.0f));
    EXPECT_TRUE(abs(v2.x - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v2.y - 1.225f) <= EPSILON);
    EXPECT_TRUE(abs(v2.z - 0.707f) <= EPSILON);

    vec3_t v3 = v.rotate_x(deg_to_rad(167.0f));
    EXPECT_TRUE(abs(v3.x - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v3.y - -1.199f) <= EPSILON);
    EXPECT_TRUE(abs(v3.z - -0.749f) <= EPSILON);

    vec3_t v4 = v.rotate_x(deg_to_rad(300.0f));
    EXPECT_TRUE(abs(v4.x - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v4.y - 1.366f) <= EPSILON);
    EXPECT_TRUE(abs(v4.z - -0.366f) <= EPSILON);
}

TEST(Vector, vec3_rotate_y)
{
    vec3_t v = {1, 1, 1};
    vec3_t v1 = v.rotate_y(deg_to_rad(15.0f));
    EXPECT_TRUE(abs(v1.x - 0.707f) <= EPSILON);
    EXPECT_TRUE(abs(v1.y - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v1.z - 1.225f) <= EPSILON);

    vec3_t v2 = v.rotate_y(deg_to_rad(-15.0f));
    EXPECT_TRUE(abs(v2.x - 1.225f) <= EPSILON);
    EXPECT_TRUE(abs(v2.y - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v2.z - 0.707f) <= EPSILON);

    vec3_t v3 = v.rotate_y(deg_to_rad(167.0f));
    EXPECT_TRUE(abs(v3.x - -1.199f) <= EPSILON);
    EXPECT_TRUE(abs(v3.y - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v3.z - -0.749f) <= EPSILON);

    vec3_t v4 = v.rotate_y(deg_to_rad(300.0f));
    EXPECT_TRUE(abs(v4.x - 1.366f) <= EPSILON);
    EXPECT_TRUE(abs(v4.y - 1.0f) <= EPSILON);
    EXPECT_TRUE(abs(v4.z - -0.366f) <= EPSILON);
}

TEST(Vector, vec3_rotate_z)
{
    vec3_t v = {1, 1, 1};
    vec3_t v1 = v.rotate_z(deg_to_rad(15.0f));
    EXPECT_TRUE(abs(v1.x - 0.707f) <= EPSILON);
    EXPECT_TRUE(abs(v1.y - 1.225f) <= EPSILON);
    EXPECT_TRUE(abs(v1.z - 1.0f) <= EPSILON);

    vec3_t v2 = v.rotate_z(deg_to_rad(-15.0f));
    EXPECT_TRUE(abs(v2.x - 1.225f) <= EPSILON);
    EXPECT_TRUE(abs(v2.y - 0.707f) <= EPSILON);
    EXPECT_TRUE(abs(v2.z - 1.0f) <= EPSILON);

    vec3_t v3 = v.rotate_z(deg_to_rad(167.0f));
    EXPECT_TRUE(abs(v3.x - -1.199f) <= EPSILON);
    EXPECT_TRUE(abs(v3.y - -0.749f) <= EPSILON);
    EXPECT_TRUE(abs(v3.z - 1.0f) <= EPSILON);

    vec3_t v4 = v.rotate_z(deg_to_rad(300.0f));
    EXPECT_TRUE(abs(v4.x - 1.366f) <= EPSILON);
    EXPECT_TRUE(abs(v4.y - -0.366f) <= EPSILON);
    EXPECT_TRUE(abs(v4.z - 1.0f) <= EPSILON);
}