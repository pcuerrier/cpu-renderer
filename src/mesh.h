#pragma once
#include "vector.h"
#include "triangle.h"

const int N_MESH_VERTICES = 8;
const int N_MESH_FACES = 6 * 2;

extern vec3_t mesh_vertices[N_MESH_VERTICES];
extern face_t mesh_faces[N_MESH_FACES];
