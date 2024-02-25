#version 460
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 uv;

out vec3 FragPos;
out vec3 outColor;
out vec3 Normal;
out vec2 UV;

uniform vec4 Color;
uniform float time;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Model;

const float waveStrength = 0.3; 
const float shininess = 64.0;   
const vec3 waterColor = vec3(0.1, 0.5, 0.7);

void main()
{
    float waveDisplacement = sin(time + vPosition.x * 0.1 + vPosition.z * 0.1) * waveStrength;

    vec3 finalFragPos = vPosition;
    finalFragPos.y += waveDisplacement;
    vec4 worldPos = Model * vec4(finalFragPos, 1.0);
    FragPos = worldPos.xyz; 

    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    Normal = normalMatrix * vNormal;

    UV = uv;
    outColor = waterColor;
    gl_Position = ProjectionMatrix * ViewMatrix * worldPos;
}