#pragma once

class Framebuffer
{
public:
	unsigned int fbo;
	unsigned int finalTexture;
	unsigned int depthRenderbuffer;

	int width;
	int height;

	Framebuffer();
};