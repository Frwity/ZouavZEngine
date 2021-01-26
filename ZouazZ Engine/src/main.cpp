#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include "Render.hpp"
#include "Resource.hpp"
#include "Rendering/Mesh.hpp"
#include <filesystem>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main()
{
    Render render(800, 600);

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

    Mesh mesh("resources/fantasy_game_inn.obj");

    Texture texture("resources/fantasy_game_inn_diffuse.png");

    while (glfwWindowShouldClose(render.window) == false)
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        texture.Use();
        shader.Use();
        glBindVertexArray(mesh.getID());
        glDrawElements(GL_TRIANGLES, mesh.getNbPoly(), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(render.window);

    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    render.Destroy();

	return EXIT_SUCCESS;
}