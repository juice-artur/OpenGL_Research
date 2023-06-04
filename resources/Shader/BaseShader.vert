#version 460
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

layout (location = 0) out vec3 outColor;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Model;
void main() {
	gl_Position = ProjectionMatrix * ViewMatrix * Model * vec4(vPosition, 1.0f);
	outColor = vColor;
}