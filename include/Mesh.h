#ifndef MESH_H
#define MESH_H

#include <glm/mat3x3.hpp>
#include <vector>
#include "tiny_obj_loader.h"
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    // glm::vec3 color;
    // glm::vec2 uv;
};

class  Mesh
{
public:
    std::vector<Vertex> vertices;

    glm::vec4 Color = {0.0f, 1.0f, 0.0f, 1.0f};

    unsigned int VAO, VBO;

    bool LoadFromObj(const char* filename);
    Mesh(const char* filename);
   // Mesh();
    void SetColor(glm::vec4 Color);

    void SetPosition(glm::vec3 Position);
    void ScaleMesh(glm::vec3 Scale);
    glm::mat4 GetModelMatrix();
    

    void Move(glm::vec3 Offset);

private:
    void SetupMesh();
    glm::mat4 CalculateModelMatrix();

    glm::vec3 Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = glm::vec3(1.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
};
#endif