#ifndef MESH_H
#define MESH_H

#include <glm/mat3x3.hpp>
#include <vector>
#include "tiny_obj_loader.h"
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
};

struct Mesh
{
    std::vector<Vertex> vertices;

    unsigned int VAO, VBO;

    bool LoadFromObj(const char* filename);
    Mesh(const char* filename);

private:
    void SetupMesh();
};
#endif