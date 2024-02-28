#include "triangle.h"

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
    vec2_t ab = b - a;
    vec2_t ac = c - a;
    vec2_t ap = p - a;
    vec2_t pb = b - p;
    vec2_t pc = c - p;

    // Area of full parallelogram (triangle ABC) using cross-product
    float area_parallelogram = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

    // Alpha = area of parallelogram-PBC over the full parallelogram-ABC
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram;
    // Beta = area of parallelogram-APC over the full parallelogram-ABC
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram;

    float gamma = 1.0f - alpha - beta;

    return { alpha, beta, gamma };
}