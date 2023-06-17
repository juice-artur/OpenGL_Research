#version 460
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;


out vec3 FragPos;
out vec3 outColor;
out vec3 Normal;

uniform vec4 Color;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Model;

void main() {

	vec4 worldPos = Model * vec4(vPosition, 1.0);
    FragPos = worldPos.xyz; 

	mat3 normalMatrix = transpose(inverse(mat3(Model)));
    Normal = normalMatrix * vNormal;


	outColor = vec3(Color);

	gl_Position = ProjectionMatrix * ViewMatrix * worldPos;
}