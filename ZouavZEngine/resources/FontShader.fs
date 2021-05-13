#version 330

out vec4 FragColor;

in vec3 Pos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 Color;

uniform sampler2D fontAtlas;

void main()
{
	FragColor = Color * texture(fontAtlas, TexCoord).a;
}