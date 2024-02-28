#include "mesh.h"
#include <stdio.h>
#include <cstring>

mesh_t mesh;

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { -1.0f, -1.0f, -1.0f }, // 1
    { -1.0f,  1.0f, -1.0f }, // 2
    {  1.0f,  1.0f, -1.0f }, // 3
    {  1.0f, -1.0f, -1.0f }, // 4
    {  1.0f,  1.0f,  1.0f }, // 5
    {  1.0f, -1.0f,  1.0f }, // 6
    { -1.0f,  1.0f,  1.0f }, // 7
    { -1.0f, -1.0f,  1.0f }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { 1, 2, 3, { 0, 1 }, { 0, 0 }, { 1, 0 }, 0xFFFFFFFF },
    { 1, 3, 4, { 0, 1 }, { 1, 0 }, { 1, 1 }, 0xFFFFFFFF },
    // right
    { 4, 3, 5, { 0, 1 }, { 0, 0 }, { 1, 0 }, 0xFFFFFFFF },
    { 4, 5, 6, { 0, 1 }, { 1, 0 }, { 1, 1 }, 0xFFFFFFFF },
    // back
    { 6, 5, 7, { 0, 1 }, { 0, 0 }, { 1, 0 }, 0xFFFFFFFF },
    { 6, 7, 8, { 0, 1 }, { 1, 0 }, { 1, 1 }, 0xFFFFFFFF },
    // left
    { 8, 7, 2, { 0, 1 }, { 0, 0 }, { 1, 0 }, 0xFFFFFFFF },
    { 8, 2, 1, { 0, 1 }, { 1, 0 }, { 1, 1 }, 0xFFFFFFFF },
    // top
    { 2, 7, 5, { 0, 1 }, { 0, 0 }, { 1, 0 }, 0xFFFFFFFF },
    { 2, 5, 3, { 0, 1 }, { 1, 0 }, { 1, 1 }, 0xFFFFFFFF },
    // bottom
    { 6, 8, 1, { 0, 1 }, { 0, 0 }, { 1, 0 }, 0xFFFFFFFF },
    { 6, 1, 4, { 0, 1 }, { 1, 0 }, { 1, 1 }, 0xFFFFFFFF }
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        mesh.vertices.push_back(cube_vertex);
    }
    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        mesh.faces.push_back(cube_face);
    }
}

static void parse_face(char* line, mesh_t& out_mesh, const std::vector<tex2_t>& texcoords)
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
    int vertex_indices[3] = {};
    int texture_indices[3] = {};
    int normal_indices[3] = {};
    switch (nbSlashes)
    {
        case 0:
        {
#ifdef WIN32
            sscanf_s(line, "f %d %d %d", &face.a, &face.b, &face.c);
#else
            sscanf(line, "f %d %d %d", &face.a, &face.b, &face.c);
#endif
        } break;

        case 1: // Texture coords
        {
#ifdef WIN32
            sscanf_s(line, "f %d/%d %d/%d %d/%d",
                &vertex_indices[0], &texture_indices[0],
                &vertex_indices[1], &texture_indices[1],
                &vertex_indices[2], &texture_indices[2]);
#else
            sscanf(line, "f %d/%d %d/%d %d/%d",
                &vertex_indices[0], &texture_indices[0],
                &vertex_indices[1], &texture_indices[1],
                &vertex_indices[2], &texture_indices[2]);
#endif
        } break;

        // TODO: Probably broken for v//vn
        case 2: // Texture coords and vertex normals
        {
#ifdef WIN32
            sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]);
#else
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]);
#endif
        } break;
    }
    face.a = vertex_indices[0] - 1;
    face.b = vertex_indices[1] - 1;
    face.c = vertex_indices[2] - 1;
    face.a_uv = texcoords[texture_indices[0] - 1];
    face.b_uv = texcoords[texture_indices[1] - 1];
    face.c_uv = texcoords[texture_indices[2] - 1];
    out_mesh.faces.push_back(face);
}

static void parse_texture_coords(char* line, mesh_t& out_mesh, std::vector<tex2_t>& texcoords)
{
    tex2_t texcoord = {};
#ifdef WIN32
    sscanf_s(line, "vt %f %f", &texcoord.u, &texcoord.v);
#else
    sscanf(line, "vt %f %f", &vertex.x, &vertex.y);
#endif
    texcoords.push_back(texcoord);
}

static void parse_vertex_index(char* line, mesh_t& out_mesh)
{
    vec3_t vertex = {};
#ifdef WIN32
    sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
#else
    sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
#endif
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
    std::vector<tex2_t> texcoords;
    while (fgets(line, 1024, file_ptr)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            out_mesh.vertices.push_back(vertex);
        }
        // Texture coordinate information
        if (strncmp(line, "vt ", 3) == 0) {
            tex2_t texcoord;
            sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
            texcoords.push_back(texcoord);
        }
        // Face information
        if (strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );
            face_t face = {};
            face.a = vertex_indices[0],
            face.b = vertex_indices[1],
            face.c = vertex_indices[2],
            face.a_uv = texcoords[texture_indices[0] - 1],
            face.b_uv = texcoords[texture_indices[1] - 1],
            face.c_uv = texcoords[texture_indices[2] - 1],
            face.color = 0xFFFFFFFF;
            out_mesh.faces.push_back(face);
        }
    }

    fclose(file_ptr);
    result = true;
    return result;
}