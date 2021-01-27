#pragma once
#include "Resource.hpp"
#include "Maths/Matrix4.hpp"
#include <string>
class Shader: public Resource
{
public:
	unsigned int id = 0;
	Shader() {};
	Shader(const char* vertexPath, const char* fragmentPath);
	const char* LoadFile(const char* pathToFile);
	void DebugCheck(const int& shader, const char* msg, const char* pathToFile);
	unsigned int CreateVertexShader(const char* vertexPath);
	unsigned int CreateFragmentShader(const char* fragmentPath);
	void Use();
	~Shader();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVector3(const std::string& name, float x, float y, float z) const;
	void SetVector3(const std::string& name, const Vec3& v) const;
	void SetMatrix(const std::string& name, const Matrix4& mat) const;
};