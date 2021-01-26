#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Resource.hpp"
#include "Rendering/Render.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void InputManager(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    camera.UpdateRotation({ (float)cursorX, (float)cursorY });

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveTo({ 0.0f, 0.0f, -deltaTime * 100 });

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveTo({ 0.0f, 0.0f, deltaTime * 100 });

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveTo({ deltaTime * 100, 0.0f, 0.0f });

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveTo({ -deltaTime * 100, 0.0f, 0.0f });
}

int main()
{
    Render render(1400, 900);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(render.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader shader("resources/shader.vs", "resources/shader.fs");
    shader.Use();
    glUniform1i(glGetUniformLocation(shader.id, "textureSample"), 0);

    Mesh mesh("resources/Skull.obj");

    //Mesh mesh;
    //mesh.CreateQuad();

    Texture texture("resources/skull.jpg");

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    Camera camera({ (float)startCursorX, (float)startCursorY });

    Matrix4 projection = Matrix4::CreatePerspectiveProjectionMatrix(render.width, render.height, 0.01, 1000, 45);

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    while (glfwWindowShouldClose(render.window) == false)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        InputManager(render.window, camera, deltaTime);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        texture.Use();
        shader.Use();

        shader.SetMatrix("view", camera.GetMatrix());
        shader.SetMatrix("projection", projection);
        //shader.SetMatrix("projection", Matrix4::Identity());
        shader.SetMatrix("model", Matrix4::Identity());

        glBindVertexArray(mesh.GetID());
        glDrawElements(GL_TRIANGLES, mesh.GetNbElements(), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(render.window);

    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    render.Destroy();

	return EXIT_SUCCESS;
}