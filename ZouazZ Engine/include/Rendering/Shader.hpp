#pragma once
#include "Resource.hpp"

class Shader: public Resource
{
public:
	unsigned int id = 0;
	Shader(const char* vertexPath, const char* fragmentPath);
	const char* LoadFile(const char* pathToFile);
	void DebugCheck(const int& shader, const char* msg, const char* pathToFile);
	unsigned int CreateVertexShader(const char* vertexPath);
	unsigned int CreateFragmentShader(const char* fragmentPath);
	void Use();
	~Shader();
};