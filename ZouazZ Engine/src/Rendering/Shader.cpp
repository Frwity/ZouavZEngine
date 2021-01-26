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