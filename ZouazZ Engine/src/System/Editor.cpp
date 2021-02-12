#include "System/Editor.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Render.hpp"
#include "Rendering/Framebuffer.hpp"

Editor::Editor()
{

}

void Editor::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::DisplayMainWindow()
{
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, .0), ImGuiCond_FirstUseEver);
    //Main editor window
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y));
    ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
    DisplayMenuBar();
    ImGui::End();
}

void Editor::DisplayMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Open", NULL);
            ImGui::MenuItem("Save", NULL);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("XXXX", NULL);
            ImGui::MenuItem("YYYY", NULL);
            ImGui::EndMenu();
        }
    }
    ImGui::EndMenuBar();
}

void Editor::FileMenu()
{

}

void Editor::Update()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Editor::DisplaySceneWindow(const Render& _render, Framebuffer& _framebuffer)
{
    //TODO display scene window
    if (ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoScrollbar))
    {
        if (ImGui::IsWindowFocused())
            glfwSetInputMode(_render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //glfwSetInputMode(_render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        ImVec2 windowSize = ImGui::GetWindowSize();

        if ((int)windowSize.x != _framebuffer.getWidth() || (int)windowSize.y != _framebuffer.getHeight())
            _framebuffer.Resize(windowSize.x, windowSize.y);

        ImGui::Image((ImTextureID)_framebuffer.getTexture(), ImVec2(_framebuffer.getWidth(), _framebuffer.getHeight()), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }
}

void Editor::AddWindow(std::string windowName)
{

}