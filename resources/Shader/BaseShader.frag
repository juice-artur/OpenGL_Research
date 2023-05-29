#version 460

layout (location = 0) in vec3 inColor;

layout (location = 0) out vec4 outFragColor;

void main()
{
	//return color
	outFragColor = vec4(1.0, 1.0, 1.0, 1.0f);
}