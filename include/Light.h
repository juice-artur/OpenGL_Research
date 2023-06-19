#ifndef LIGHT_H
#define LIGHT_H

#include <Mesh.h>
#include <glm/fwd.hpp>

class Light
{
public:
    Light(Mesh _Mesh, glm::vec3 Position, glm::vec4 Color);
    Light(Mesh _Mesh, glm::vec3 Position, glm::vec4 Color, float Linear, float Quadratic);
    // TODO: It is worth thinking about what can be done to correctly install the mesh for the light source, because the mesh also has a
    // position
    void SetMesh(Mesh Mesh);

    glm::vec3 GetPosition() const;
    glm::vec4 GetColor() const;

    float GetLinear() const;
    float GetQuadratic() const;

    float GetRadius() const;

private:
    Mesh _Mesh;
    glm::vec3 Position;
    glm::vec4 Color;

    float Linear;
    float Quadratic;
    float Radius;
};
#endif