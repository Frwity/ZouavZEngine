#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 color;

uniform sampler2D fontAtlas;

uniform float width;
uniform float edge;

uniform float outlineWidth;
uniform float outlineEdge;
uniform vec3 outlineColor;

uniform vec2 offset;

void main()
{
	float distance = 1.0 - texture(fontAtlas, TexCoord).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);

	float outlineDistance = 1.0 - texture(fontAtlas, TexCoord + offset).a;
	float outlineAlpha = 1.0 - smoothstep(outlineWidth, outlineWidth + outlineEdge, outlineDistance);

	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColor = mix(outlineColor, color.xyz, alpha / overallAlpha);
	
	if (overallAlpha <= 0.1)
		discard;
	FragColor = vec4(overallColor, overallAlpha);
}