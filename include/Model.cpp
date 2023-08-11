#include "Model.h"

void Model::SetPosition(const glm::vec3& newPosition) 
{
    for (Mesh& mesh : meshes)
    {
        mesh.SetPosition(newPosition);
    }
}

void Model::SetColor(const glm::vec4& newColor) 
{
    for (Mesh& mesh : meshes)
    {
        mesh.SetColor(newColor);
    }
}

void Model::ScaleModel(const glm::vec3& Scale) 
{
    for (Mesh& mesh : meshes)
    {
        mesh.ScaleMesh(Scale);
    }
}

void Model::RotateModel(const glm::vec3& Rotate) 
{
    for (Mesh& mesh : meshes)
    {
        mesh.RotateMesh(Rotate);
    }
}
