#version 460

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gColor;

in vec3 FragPos;
in vec3 outColor;
in vec3 Normal;

void main()
{
	gPosition = FragPos;
	gColor = outColor;
    gNormal = normalize(Normal);
}