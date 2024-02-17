#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

// TODO: Move to own io file
// if typedef doesn't exist (msvc, blah)
typedef intptr_t ssize_t;
// https://stackoverflow.com/questions/735126/are-there-alternate-implementations-of-gnu-getline-interface
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = (char*)malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = (char*)realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = (unsigned char)c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

static void parse_face(char* line, size_t len, mesh_t& out_mesh)
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
    char* ptr_start = line;
    char* ptr_end = nullptr;
    int idx = 0;
    face_t face = {};

    while (ptr_end < (line + len) && idx < 3)
    {
        // Temporarily advance ptr until next space
        while (ptr_start[0] != ' ') { ++ptr_start; }
        ++ptr_start;

        int indice = strtol(ptr_start, &ptr_end, 10);
        face.data[idx++] = indice - 1;
        ptr_start = ptr_end;
    }
    out_mesh.faces.push_back(face);
}

static void parse_vertex_index(char* line, size_t len, mesh_t& out_mesh)
{
    char* ptr_start = line;
    char* ptr_end = nullptr;
    int idx = 0;
    vec3_t vertex = {};
    while (ptr_end < (line + len) && idx < 3)
    {
        // Temporarily advance ptr until next space
        while (ptr_start[0] != ' ') { ++ptr_start; }
        ++ptr_start;

        int whole = strtol(ptr_start, &ptr_end, 10);
        float fraction = ptr_end[0] == '.' ? (float)strtol(ptr_end, &ptr_end, 10) : 0.0f;
        while (fraction > 1.0f)
        {
            fraction /= 10;
        }
        vertex.data[idx++] = (float)whole + fraction;
        ptr_start = ptr_end;
    }
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

    char* line = nullptr;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file_ptr)) != -1)
    {
        // Process line
        switch (line[0])
        {
            case '#': // Comment
            {
                continue;
            } break;

            case 'v':
            {
                switch (line[1])
                {
                    case ' ': // Vertex index
                    {
                        parse_vertex_index(line, len, out_mesh);
                    } break;
                }
            } break;

            case 'f': // Face
            {
                parse_face(line, len, out_mesh);
            } break;
        }
    }

    fclose(file_ptr);
    if (line)
    {
        free(line);
    }
    result = true;
    return result;
}