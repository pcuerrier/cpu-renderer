#pragma once

#include "vector.h"

#include <cstdint>

struct light_t
{
    vec3_t direction = { 0.0f, 0.0f, 0.0f };
};

uint32_t light_apply_intensity(uint32_t original_color, float percentage);
