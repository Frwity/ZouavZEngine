#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Pos;
out vec2 TexCoord;
out vec3 Normal;

uniform vec3 centerPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Pos = centerPos + vec3(view[0][0], view[1][0], view[2][0]) * aPos.x + vec3(view[0][1], view[1][1], view[2][1]) * aPos.y;
	Normal   = mat3(transpose(inverse(model))) * aNormal;     
	TexCoord = aTexCoord;

	gl_Position = projection * view * vec4(Pos, 1.0);
}

