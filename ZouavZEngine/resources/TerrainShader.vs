#version 330 core

#define MAX_COLOR 8

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Pos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Color;

uniform int colorCount;
uniform float colorHeight[MAX_COLOR];
uniform vec3 colors[MAX_COLOR];

uniform float minHeight;
uniform float maxHeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Pos      = vec3(model * vec4(aPos, 1.0));
	Normal   = mat3(transpose(inverse(model))) * aNormal;     
	TexCoord = aTexCoord;

	Color = colors[0];
	float drawStrength = 0;
	float heightPercent = smoothstep(minHeight, maxHeight, Pos.y);

	for(int i = 0; i < colorCount; i++)
	{
		drawStrength = clamp(sign(heightPercent - colorHeight[i]), 0.0, 1.0);
		Color = Color * (1 - drawStrength) + colors[i] * drawStrength;
	}

	gl_Position = projection * view * vec4(Pos, 1.0);
}