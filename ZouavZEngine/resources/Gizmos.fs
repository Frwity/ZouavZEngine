#version 330 core

in vec3 Pos;
in vec4 Color;

out vec4 FragColor;

void main()
{
	FragColor = Color;
}