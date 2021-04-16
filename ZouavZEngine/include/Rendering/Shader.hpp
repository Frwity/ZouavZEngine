#pragma once
#include "Resource.hpp"
#include <string>
#include <vector>

class Mat4;
class Vec3;
class Vec2;

class Shader: public Resource
{
public:
	unsigned int id = 0;
	Shader(const std::string& _name, const char* vertexPath, const char* fragmentPath);
	const char* LoadFile(const char* pathToFile);
	void DebugCheck(const int& shader, const char* msg, const char* pathToFile);
	unsigned int CreateVertexShader(const char* vertexPath);
	unsigned int CreateFragmentShader(const char* fragmentPath);
	void Use();
	~Shader();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetFloatArray(const std::string& name, float* value, unsigned int size) const;
	void SetVector3(const std::string& name, float x, float y, float z) const;
	void SetVector3(const std::string& name, const Vec3& v) const;
	void SetVector2(const std::string& name, const Vec2& v) const;
	void SetMatrix(const std::string& name, const Mat4& mat) const;
	void SetLight(const std::vector<class Light*>& lights);
};