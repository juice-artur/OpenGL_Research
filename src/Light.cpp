#include "Light.h"


Light::Light(Mesh _Mesh, glm::vec3 Position, glm::vec4 Color)
{
    this->_Mesh = _Mesh;
    this->Position = Position;
    this->Color = Color;
    this->Linear = 0.7;
    this->Quadratic = 1.8;
    const float Constant = 1.0;
    const float maxBrightness = std::fmaxf(std::fmaxf(this->Color.r, this->Color.g), this->Color.b);
    this->Radius =
        (-Linear + std::sqrt(Linear * Linear - 4 * Quadratic * (Constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * Quadratic);
}

Light::Light(Mesh _Mesh, glm::vec3 Position, glm::vec4 Color, float Linear, float Quadratic)
{
    this->_Mesh = _Mesh;
    this->Position = Position;
    this->Color = Color;
    this->Linear = Linear;
    this->Quadratic = Quadratic;
    const float Constant = 1.0;
    const float maxBrightness = std::fmaxf(std::fmaxf(this->Color.r, this->Color.g), this->Color.b);
    this->Radius =
        (-Linear + std::sqrt(Linear * Linear - 4 * Quadratic * (Constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * Quadratic);
}

void Light::SetMesh(Mesh _Mesh) 
{
    this->_Mesh = _Mesh;
    this->_Mesh.SetPosition(this->Position);
    this->_Mesh.SetColor(this->Color);
}

glm::vec3 Light::GetPosition()
{
    return Position;
}

glm::vec4 Light::GetColor()
{
    return Color;
}
