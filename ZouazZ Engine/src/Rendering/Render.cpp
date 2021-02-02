#include "Rendering/Render.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Mesh.hpp"


Render::Render(int _width, int _height)
    : width(_width), height(_height)
{
      // Init glfw
    if (!glfwInit())
    {
        fprintf(stderr, "glfwInit failed");
        //TODO EXIT WITH ERROR
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "ZouavZ Engine", nullptr, nullptr);
    if (window == nullptr)
    {
        fprintf(stderr, "glfwCreateWindow failed");
        //TODO EXIT WITH ERROR
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        fprintf(stderr, "gladLoadGLLoader failed");
        //TODO EXIT WITH ERROR
    }

    glViewport(0, 0, width, height);

    //TODO PUT IN DEBUG
    printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION = %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Render::Init(int _width, int _height)
{
    width = _width;
    height = _height;

    // Init glfw
    if (!glfwInit())
    {
        fprintf(stderr, "glfwInit failed");
        //TODO EXIT WITH ERROR
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "ZouavZ Engine", nullptr, nullptr);
    if (window == nullptr)
    {
        fprintf(stderr, "glfwCreateWindow failed");
        //TODO EXIT WITH ERROR
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        fprintf(stderr, "gladLoadGLLoader failed");
        //TODO EXIT WITH ERROR
    }

    //TODO PUT IN DEBUG
    printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION = %s\n", glGetString(GL_VERSION));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, (int)width / 2, (int)height / 2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Render::Update()
{
    glfwSwapBuffers(window);
}

bool Render::Stop()
{
    return glfwWindowShouldClose(window);
}

void Render::Clear()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::Draw(const Mesh& _mesh)
{
    glBindVertexArray(_mesh.GetID());
    glDrawElements(GL_TRIANGLES, _mesh.GetNbElements(), GL_UNSIGNED_INT, 0);
}

void Render::Destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}