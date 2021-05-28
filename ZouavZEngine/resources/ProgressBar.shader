#ifdef COMPILING_VS

layout (location = 0) in vec3 aPos;

out float fPosX;

uniform mat4 projection;

void main()
{
    fPosX = aPos.x;
	gl_Position = projection * vec4(aPos.x, aPos.y, 1.0, 1.0);
}

#endif

#ifdef COMPILING_FS

out vec4 FragColor;

in float fPosX;

uniform float sizeFilled;
uniform vec4 backgroudColor;
uniform vec4 fillColor;

void main()
{
    if (fPosX < sizeFilled)
	    FragColor = fillColor;
    else
	    FragColor = backgroudColor; 
}

#endif