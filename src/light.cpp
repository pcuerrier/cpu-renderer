#include "light.h"

uint32_t light_apply_intensity(uint32_t original_color, float percentage)
{
    if (percentage < 0.0f)
    {
        return 0x0f; // completely black
    }

    if (percentage > 1.0f)
    {
        return original_color;
    }

    uint32_t a = (original_color & 0xFF000000);
    uint32_t r = (uint32_t)((original_color & 0x00FF0000) * percentage);
    uint32_t g = (uint32_t)((original_color & 0x0000FF00) * percentage);
    uint32_t b = (uint32_t)((original_color & 0x000000FF) * percentage);
    return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}
