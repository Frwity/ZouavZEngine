#include "System/Editor.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <filesystem>

bool newFolderWindow = false;
char folderName[256] = "New Folder";
bool newFileWindow = false;
char fileName[256] = "New File";
std::string actualFolder = ".";

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
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
    //Main editor window
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y));
    ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
    DisplayMenuBar();
    ImGui::End();
}

void ListActualFolder()
{
    if (ImGui::Button("../"))
        actualFolder.append("/../");

    for (const auto& entry : std::filesystem::directory_iterator(actualFolder))
    {
        if (entry.is_directory())
        {
            if (ImGui::Button(entry.path().string().c_str()))
                actualFolder = entry.path().string();
        }
        else
            ImGui::Text(entry.path().string().c_str());
    }
}

void Editor::DisplayMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Folder", nullptr, &newFolderWindow);
            ImGui::MenuItem("New File", nullptr, &newFileWindow);
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

    if (newFolderWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f));
        ImGui::Begin("New Folder", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

        ListActualFolder();

        ImGui::InputText("Folder Name", folderName, 256);

        if (ImGui::Button("Create"))
        {
            if (_mkdir(std::string(actualFolder).append("/").append(folderName).c_str()) == 0)
                std::cout << "Folder " << folderName << " created" << std::endl;
            else
                std::cout << "Folder " << folderName << " not created" << std::endl;
            newFolderWindow = !newFolderWindow;
        }
        ImGui::End();
    }

    if (newFileWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f));
        ImGui::Begin("New File", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

        ListActualFolder();

        ImGui::InputText("File Name", fileName, 256);

        if (ImGui::Button("Create"))
        {
            if (std::fstream(std::string(actualFolder).append("/").append(fileName).c_str()))
                std::cout << "File " << fileName << " not created" << std::endl;
            else
                if (std::ofstream(std::string(actualFolder).append("/").append(fileName).c_str()))
                    std::cout << "File " << fileName << " created" << std::endl;
                else
                    std::cout << "File " << fileName << " not created " << std::string(actualFolder).append("/").append(fileName) << std::endl;
            newFileWindow = !newFileWindow;
        }
        ImGui::End();
    }
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

void Editor::DisplaySceneWindow()
{
    //TODO display scene window
}

void Editor::AddWindow(std::string windowName)
{

}