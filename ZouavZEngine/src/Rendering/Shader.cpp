#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include "Maths/Mat4.hpp"
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Vec4.hpp"
#include "Component/Light.hpp"
#include "GameObject.hpp"
#include "System/Debug.hpp"
#include "Rendering/Shader.hpp"

Shader::Shader(const std::string& _name, const char* _vertexPath, const char* _fragmentPath)
    : Resource(_name) 
{
    unsigned int vertexShader = CreateVertexShader(_vertexPath);
    unsigned int fragmentShader = CreateFragmentShader(_fragmentPath);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    paths.emplace_back(_vertexPath);
    paths.emplace_back(_fragmentPath);
}

unsigned int Shader::CreateVertexShader(const char* _vertexPath)
{
    const char* vertexShaderSource = LoadFile(_vertexPath);
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    DebugCheck(vertexShader, "VERT ERROR", _vertexPath);

    delete[] vertexShaderSource;

    return vertexShader;
}

unsigned int Shader::CreateFragmentShader(const char* _fragmentPath)
{
    const char* fragmentShaderSource = LoadFile(_fragmentPath);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    DebugCheck(fragmentShader, "FRAG ERROR", _fragmentPath);

    delete[] fragmentShaderSource;

    return fragmentShader;
}

const char* Shader::LoadFile(const char* _pathToFile)
{
    std::ifstream file(_pathToFile, std::ios::binary);

    if (file)
    {
        file.seekg(0, file.end);
        int length = (int)file.tellg();
        file.seekg(0, file.beg);

        char* buffer = new char[length + 1];

        file.read(buffer, length);
        buffer[length] = '\0';

        file.close();

        return buffer;
    }

    return nullptr;
}


void Shader::DebugCheck(const int& _shader, const char* _msg, const char* _src)
{
    int success;
    char infoLog[1024];
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
        Debug::LogWarning(std::string(_msg).append(" : ").append(_src).append("\n").append(infoLog).append("\n"));
    }
}


void Shader::Use()
{
    glUseProgram(id);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}


void Shader::SetBool(const std::string& _name, bool _value) const
{
    glUniform1i(glGetUniformLocation(id, _name.c_str()), (int)_value);
}

void Shader::SetInt(const std::string& _name, int _value) const
{
    glUniform1i(glGetUniformLocation(id, _name.c_str()), _value);
}

void Shader::SetFloat(const std::string& _name, float _value) const
{
    glUniform1f(glGetUniformLocation(id, _name.c_str()), _value);
}

void Shader::SetFloatArray(const std::string& _name, const float* _value, unsigned int _size) const
{
    glUniform1fv(glGetUniformLocation(id, _name.c_str()), _size, _value);
}

void Shader::SetMatrix(const std::string& _name, const Mat4& _mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, _name.c_str()), 1, GL_FALSE, _mat.matrix);
}

void Shader::SetVector2(const std::string& _name, const Vec2& _v) const
{
    glUniform2f(glGetUniformLocation(id, _name.c_str()), _v.x, _v.y);
}

void Shader::SetVector3(const std::string& _name, float _x, float _y, float _z) const
{
    glUniform3f(glGetUniformLocation(id, _name.c_str()), _x, _y, _z);
}

void Shader::SetVector3(const std::string& _name, const Vec3& _v) const
{
    glUniform3f(glGetUniformLocation(id, _name.c_str()), _v.x, _v.y, _v.z);
}

void Shader::SetVector4(const std::string& _name, float _w, float _x, float _y, float _z) const
{
    glUniform4f(glGetUniformLocation(id, _name.c_str()), _w, _x, _y, _z);
}

void Shader::SetVector4(const std::string& _name, const Vec4& _v) const
{
    glUniform4f(glGetUniformLocation(id, _name.c_str()), _v.w, _v.x, _v.y, _v.z);
}

void Shader::SetLight(const std::vector<Light*>& _lights)
{
    int i = 0;
    for (const Light* l : _lights)
    {
        const std::string index = std::to_string(i++);
        
        SetVector3(("lights[" + index + "].position").c_str(), l->GetGameObject().localPosition);
        SetVector3(("lights[" + index + "].ambient").c_str(), l->ambient);
        SetVector3(("lights[" + index + "].diffuse").c_str(), l->diffuse);
        SetVector3(("lights[" + index + "].specular").c_str(), l->specular);
        SetVector3(("lights[" + index + "].direction").c_str(), l->direction);
        SetVector3(("lights[" + index + "].ConstLineQuad").c_str(), l->constLineQuad);
        SetVector2(("lights[" + index + "].cutOffOuterCutOff").c_str(), l->cutOffOuterCutOff);
        SetInt(("lights[" + index + "].type").c_str(), (int)l->type);
    }
}