#include "texture.h"

#include <filesystem>
#include <fstream>

unsigned long texture_width = 0;
unsigned long texture_height = 0;

std::vector<unsigned char> png_texture;
uint32_t* mesh_texture = nullptr;

void load_png_texture_data(const char* filename) {
    std::vector<unsigned char> in;
    std::ifstream file(filename, std::ios::binary);
    if (file.fail()) {
        perror(filename);
    }

    //seek to the end
    file.seekg(0, std::ios::end);

    //Get the file size
    unsigned int fileSize = (unsigned int)file.tellg();
    file.seekg(0, std::ios::beg);

    //Reduce the file size by any header bytes that might be present
    fileSize -= (unsigned int)file.tellg();

    in.resize(fileSize);
    file.read((char *)&(in[0]), fileSize);
    file.close();
    int errorCode = decodePNG(png_texture, texture_width, texture_height, &(in[0]), in.size());
    if (errorCode != 0)
    {
        // TODO: Log error
    }
    mesh_texture = (uint32_t*)(&(png_texture[0]));
}