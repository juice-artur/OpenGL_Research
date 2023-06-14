#version 460

layout (location = 0) in vec3 inColor;

layout (location = 0) out vec4 outFragColor;

uniform vec4 Color;

void main()
{
	outFragColor = Color;
}