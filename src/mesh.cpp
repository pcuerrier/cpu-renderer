#include "mesh.h"
#include <stdio.h>
#include <cstring>
static void parse_face(char* line, mesh_t& out_mesh)
{
    // Possible Read
    // - Vertex indices only
    //      f v1 v2 v3 ...
    // - Vertex indices and texture coords indices
    //      f v1/vt1 v2/vt2 v3/vt3 ...
    // - Vertex indices and normal indices
    //      f v1//vn1 v2//vn2 v3//vn3 ...
    // - Vertex indices, texture coords indices and normal indices
    //      f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
    // Only supports 1 case for now
    // Only supports triangles (3 indices per line)
    // TODO: Add support for other cases
    // TODO: Add support for more than 3 vertices
    // TODO: Add support for index -1, referring to the last vertex
    char* ptr_start = line + 2;
    int i = 0;
    int nbSlashes = 0;
    while (ptr_start[0] != ' ' && i < 50)
    {
        if (ptr_start[0] == '/')
        {
            ++nbSlashes;
        }
        ++ptr_start;
        ++i;
    }

    face_t face = {};
    switch (nbSlashes)
    {
        case 0:
        {
            sscanf(line, "f %d %d %d", &face.a, &face.b, &face.c);
        } break;

        case 1: // Texture coords
        {
            face_t texCoords = {};
            sscanf(line, "f %d/%d %d/%d %d/%d",
                &face.a, &texCoords.a,
                &face.b, &texCoords.b,
                &face.c, &texCoords.c);
        } break;

        // TODO: Probably broken for v//vn
        case 2: // Texture coords and vertex normals
        {
            face_t texCoords = {};
            face_t normals = {};
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &face.a, &texCoords.a, &normals.a,
                &face.b, &texCoords.b, &normals.b,
                &face.c, &texCoords.c, &normals.c);   
        } break;
    }
    out_mesh.faces.push_back(face);
}

static void parse_vertex_index(char* line, mesh_t& out_mesh)
{
    vec3_t vertex = {};
    sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
    out_mesh.vertices.push_back(vertex);
}

// https://en.wikipedia.org/wiki/Wavefront_.obj_file#References
bool create_mesh_from_obj(const char* filepath, mesh_t& out_mesh)
{
    bool result = false;

    // TODO: Check if .obj file else error
#ifdef WIN32
    FILE* file_ptr = nullptr;
    fopen_s(&file_ptr, filepath, "r");
#else
    FILE* file_ptr = fopen(filepath, "r");
#endif
    if (!file_ptr)
    {
        // TODO: Log error
        return result;
    }

    char line[2024];
    while (fgets(line, 1024, file_ptr))
    {
        switch (line[0])
        {
            case 'v':
            {
                switch (line[1])
                {
                    case ' ':
                    {
                        parse_vertex_index(line, out_mesh);
                    } break;
                }
            } break;

            case 'f':
            {
                parse_face(line, out_mesh);
            } break;
        }
    }

    fclose(file_ptr);
    result = true;
    return result;
}