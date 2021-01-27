#include "Rendering/Render.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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
}

void Render::Draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Render Scene

    glfwSwapBuffers(window);
}

void Render::Destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}