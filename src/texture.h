#pragma once

#include <cstdint>
#include "picopng.h"

struct tex2_t
{
    float u = 0.0f;
    float v = 0.0f;
};

extern unsigned long texture_width;
extern unsigned long texture_height;

extern std::vector<unsigned char> png_texture;
extern uint32_t* mesh_texture;

void load_png_texture_data(const char* filename);