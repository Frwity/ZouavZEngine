#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include "Rendering/Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    unsigned int vertexShader = CreateVertexShader(vertexPath);
    unsigned int fragmentShader = CreateFragmentShader(fragmentPath);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shader::CreateVertexShader(const char* vertexPath)
{
    const char* vertexShaderSource = LoadFile(vertexPath);
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    DebugCheck(vertexShader, "VERT ERROR", vertexPath);

    delete[] vertexShaderSource;

    return vertexShader;
}

unsigned int Shader::CreateFragmentShader(const char* fragmentPath)
{
    const char* fragmentShaderSource = LoadFile(fragmentPath);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    DebugCheck(fragmentShader, "FRAG ERROR", fragmentPath);

    delete[] fragmentShaderSource;

    return fragmentShader;
}

const char* Shader::LoadFile(const char* pathToFile)
{
    std::ifstream file(pathToFile, std::ios::binary);

    if (file)
    {
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        char* buffer = new char[length + 1];

        file.read(buffer, length);
        buffer[length] = '\0';

        file.close();

        return buffer;
    }

    return nullptr;
}


void Shader::DebugCheck(const int& shader, const char* msg, const char* src)
{
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << msg << " : " << src << std::endl << infoLog << std::endl;
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


void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetMatrix(const std::string& name, const Matrix4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, mat.matrix);
}

void Shader::SetVector3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::SetVector3(const std::string& name, const Vec3& v) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}