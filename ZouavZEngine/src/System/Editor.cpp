#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Rendering/Render.hpp"
#include "Rendering/Framebuffer.hpp"
#include "Component/MeshRenderer.hpp"
#include "System/InputManager.hpp"
#include "System/Debug.hpp"
#include "Scene.hpp"
#include "System/TimeManager.hpp"
#include "System/Engine.hpp"
#include "System/Editor.hpp"

bool newFolderWindow = false;
char folderName[256] = "New Folder";
bool newFileWindow = false;
char fileName[256] = "New File";
bool newClassWindow = false;
char className[256] = "NewClass";
std::string actualFolder = ".";
bool hierarchyMenu = false;
ImVec2 hierarchyMenuPos = { 0.0f, 0.0f };
char newGameObjectName[256] = "New GameObject";
GameObject* newGameObjectParent = nullptr;
GameObject* selectedGameObject = nullptr;
GameObject* gameObjectInspector = nullptr;

bool consoleText = true;
bool consoleWarning = true;
bool consoleError = true;

static ImGuiID dockspaceID = 1;

Editor::Editor(class Engine& _engine)
    : engine(_engine)
{
    isKeyboardEnable = false;
}

void Editor::Init()
{
    sceneCamera = SceneCamera(engine.render.width, engine.render.height);
    sceneCamera.SetSceneCamera();
    imguiStyle = &ImGui::GetStyle();

    editorClock = TimeManager::CreateClock();
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
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
    ImGui::End();
}

std::string GetRightName(const std::string& _str)
{
    return _str.substr(_str.find_last_of("/\\") + 1);
}

void Editor::DisplayOptionWindow()
{
	ImGui::Begin("Option", NULL, ImGuiWindowFlags_NoMove
                                
                                | ImGuiWindowFlags_NoNav
                                | ImGuiWindowFlags_NoBackground
		                        | ImGuiWindowFlags_NoScrollWithMouse
		                        | ImGuiWindowFlags_NoTitleBar
		                        | ImGuiWindowFlags_NoScrollbar
                                | ImGuiWindowFlags_NoCollapse
		                        | ImGuiWindowFlags_NoResize
    );

	ImGui::SameLine();
	ImGui::SetCursorPosX(800);

	if (ImGui::Button("Play"))
	{
	    state = EDITOR_STATE::PLAYING;
        TimeManager::gameClock->Activate();
	    imguiStyle->Colors[ImGuiCol_WindowBg] = ImVec4(3.0f, 0.0f, 0.0f, 0.5f);
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {
        state = EDITOR_STATE::PAUSE;
        TimeManager::gameClock->Dehactivate();

    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        state = EDITOR_STATE::EDITING;
        TimeManager::gameClock->Reset();

        imguiStyle->Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
	}


	ImGui::End();
}

void ListActualFolder(bool& windowOpened)
{
    if (ImGui::Button("../"))
        actualFolder.append("/../");

    ImGui::SameLine(ImGui::GetWindowWidth() - 20.0f);
    if (ImGui::Button("X"))
        windowOpened = !windowOpened;

    std::string currentName;

    for (const auto& entry : std::filesystem::directory_iterator(actualFolder))
    {
        currentName = entry.path().string();
        if (entry.is_directory())
        {
            if (ImGui::Button(GetRightName(currentName).c_str()))
                actualFolder = currentName;
            ImGui::SameLine();
            if (ImGui::Button(std::string("Remove##").append(currentName).c_str()))
            {
                if (_rmdir(currentName.c_str()) == 0)
                {
                    std::cout << currentName << " removed" << std::endl;
                    windowOpened = !windowOpened;
                }
                else
                    std::cout << currentName << " not removed" << std::endl;
            }
        }
        else
        {
            ImGui::Text(GetRightName(currentName).c_str());
            ImGui::SameLine();
            if (ImGui::Button(std::string("Remove##").append(currentName).c_str()))
            {
                if (std::remove(currentName.c_str()))
                {
                    std::cout << currentName << " removed" << std::endl;
                    windowOpened = !windowOpened;
                }
                else
                    std::cout << currentName << " not removed" << std::endl;
            }
        }
    }
}

void CreateNewClass(std::string className)
{
    std::ofstream hppFile(std::string("include/Game/").append(className).append(".hpp").c_str());
    std::ofstream cppFile(std::string("src/Game/").append(className).append(".cpp").c_str());

    if (hppFile && cppFile)
    {
        hppFile <<
            "#pragma once\n"
            "#include \"Component/ScriptComponent.hpp\"\n\n"
            "class " << className << " : public ScriptComponent\n"
            "{\n"
            "   public:\n"
            "        " << className << "() = delete;\n"
            "        " << className << "(class GameObject* _gameobject);\n"
            "        void Begin() final;\n"
            "        void Update() final;\n"
            "};\0";

        cppFile <<
            "#include \"Game/" << className << ".hpp\"\n"
            "#include \"GameObject.hpp\"\n\n"

            << className << "::" << className << "(GameObject * _gameobject)\n"
            ": ScriptComponent(_gameobject)\n"
            "{}\n\n"

            "void " << className << "::Begin()\n"
            "{}\n\n"

            "void " << className << "::Update()\n"
            "{}\0";
    }
}

void NewFolderWindow()
{
    if (newFolderWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f));
        ImGui::Begin("New Folder", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

        ListActualFolder(newFolderWindow);

        ImGui::InputText("Folder Name", folderName, 256);

        if (ImGui::Button("Create"))
        {
            if (_mkdir(std::string(actualFolder).append("/").append(folderName).c_str()) == 0)
            {
                std::cout << "Folder " << folderName << " created" << std::endl;
                newFolderWindow = !newFolderWindow;
            }
            else
                std::cout << "Folder " << folderName << " not created" << std::endl;
        }
        ImGui::End();
    }
}

void NewFileWindow()
{
    if (newFileWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f));
        ImGui::Begin("New File", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

        ListActualFolder(newFileWindow);

        ImGui::InputText("File Name", fileName, 256);

        if (ImGui::Button("Create"))
        {
            if (std::fstream(std::string(actualFolder).append("/").append(fileName).c_str()))
                std::cout << "File " << fileName << " not created" << std::endl;
            else
                if (std::ofstream(std::string(actualFolder).append("/").append(fileName).c_str()))
                {
                    std::cout << "File " << fileName << " created" << std::endl;
                    newFileWindow = !newFileWindow;
                }
                else
                    std::cout << "File " << fileName << " not created " << std::string(actualFolder).append("/").append(fileName) << std::endl;
        }
        ImGui::End();
    }
}

void NewClassWindow()
{
    if (newClassWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f));
        ImGui::Begin("New Class", NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

        ImGui::InputText("Class Name", className, 256);

        if (ImGui::Button("Create"))
            CreateNewClass(className);

        ImGui::End();
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
            ImGui::MenuItem("New Class", nullptr, &newClassWindow);
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

    NewFolderWindow();

    NewFileWindow();

    NewClassWindow();
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

    sceneCamera.Update(isKeyboardEnable, editorClock->GetDeltaTime());
}

void Editor::DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer)
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        if (ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_RIGHT) && !isKeyboardEnable)
        {
            isKeyboardEnable = true;
            glfwSetInputMode(_render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(_render.window, lastCursorScenePosX, lastCursorScenePosY);
        }

        if (InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON::BUTTON_RIGHT) && isKeyboardEnable)
        {
            isKeyboardEnable = false;
            glfwGetCursorPos(_render.window, &lastCursorScenePosX, &lastCursorScenePosY);
            glfwSetInputMode(_render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ImGui::SetWindowFocus("Main");
        }

        ImVec2 windowSize = ImGui::GetWindowSize();

        if ((int)windowSize.x != _framebuffer.getWidth() || (int)windowSize.y != _framebuffer.getHeight())
            _framebuffer.Resize((int)windowSize.x, (int)windowSize.y);

        ImGui::Image((ImTextureID)_framebuffer.getTexture(), ImVec2((float)_framebuffer.getWidth(), (float)_framebuffer.getHeight()), ImVec2(0,1), ImVec2(1,0));
    }
    ImGui::End();
}

void Editor::DisplayInspector()
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        if (gameObjectInspector)
        {
            ImGui::InputText("##name", gameObjectInspector->name.data(), 256);

            ImGui::Text("World Position :");
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldPosition().x).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldPosition().y).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldPosition().z).c_str());

            ImGui::Text("World Rotation :");
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldRotation().x).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldRotation().y).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldRotation().z).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldRotation().z).c_str());

            ImGui::Text("World Scale :");
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldScale().x).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldScale().y).c_str());
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::Text(std::to_string(gameObjectInspector->WorldScale().z).c_str());

            ImGui::Text("Local Position :");
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##positionx", &gameObjectInspector->localPosition.x);
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##positiony", &gameObjectInspector->localPosition.y);
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##positionz", &gameObjectInspector->localPosition.z);

            static  Vec3 eulerAngles;// = gameObjectInspector->localRotation.ToEuler();

            ImGui::Text("Local Rotation :");
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##rotationx", &eulerAngles.x);
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##rotationy", &eulerAngles.y);
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##rotationz", &eulerAngles.z);

            if (ImGui::Button("Non"))
                gameObjectInspector->localRotation = Quaternion(eulerAngles);

            ImGui::Text("Local Scale :");
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##scalex", &gameObjectInspector->localScale.x);
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##scaley", &gameObjectInspector->localScale.y);
            ImGui::SameLine(); ImGui::PushItemWidth(70.0f); ImGui::InputFloat("##scalez", &gameObjectInspector->localScale.z);

            if (!gameObjectInspector->GetComponent<MeshRenderer>())
                if (ImGui::Button("Add Mesh Renderer"))
                    gameObjectInspector->AddComponent<MeshRenderer>();
        }
    }
    ImGui::End();
}

void Editor::DisplayConsoleWindow()
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Checkbox("Text", &consoleText);
        ImGui::SameLine(); ImGui::Checkbox("Warning", &consoleWarning);
        ImGui::SameLine(); ImGui::Checkbox("Error", &consoleError);

        for (const auto& log : Debug::logs)
        {
            switch (log.second)
            {
                case E_LOGS_TYPE::TEXT:
                    if (consoleText)
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.first.c_str());
                    break;
                case E_LOGS_TYPE::WARNING:
                    if (consoleWarning)
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), log.first.c_str());
                    break;
                case E_LOGS_TYPE::ERROR:
                    if (consoleError)
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), log.first.c_str());
                    break;
            }
        }
    }
    ImGui::End();
}

void Editor::DisplayGameWindow(const class Render& _render, class Framebuffer& _framebuffer)
{
    if (ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();

        if ((int)windowSize.x != _framebuffer.getWidth() || (int)windowSize.y != _framebuffer.getHeight())
            _framebuffer.Resize((int)windowSize.x, (int)windowSize.y);

        ImGui::Image((ImTextureID)_framebuffer.getTexture(), ImVec2((float)_framebuffer.getWidth(), (float)_framebuffer.getHeight()), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

void DisplayChild(GameObject* _parent)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf;
    
    if (selectedGameObject == _parent)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(_parent->name.c_str(), flags))
    {
        if (ImGui::IsItemHovered())
        {
            if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_RIGHT))
            {
                hierarchyMenuPos = ImGui::GetMousePos();
                hierarchyMenu = true;
                newGameObjectParent = _parent;
            }

            if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && _parent->name != "World")
                selectedGameObject = _parent;

            if (InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && selectedGameObject)
            {
                if (selectedGameObject == _parent)
                    gameObjectInspector = selectedGameObject;

                else if (!_parent->IsChildOf(selectedGameObject))
                {
                    selectedGameObject->SetParent(_parent);
                    selectedGameObject = nullptr;
                }
            }
        }
        for (GameObject* child : _parent->GetChildren())
            DisplayChild(child);

        ImGui::TreePop();
    }
}

void Editor::DisplayHierarchy()
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {
        DisplayChild(&Scene::GetCurrentScene()->GetWorld());

        if (ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_RIGHT))
        {
            hierarchyMenuPos = ImGui::GetMousePos();
            hierarchyMenu = true;
        }

        if (hierarchyMenu)
        {
            ImGui::SetNextWindowPos(hierarchyMenuPos);
            if (ImGui::Begin("Hierarchy Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
            {
                ImGui::InputText("New GameObject Name", newGameObjectName, 256);
                if (ImGui::Button("New GameObject"))
                {
                    GameObject* newGameObject = GameObject::CreateGameObject(newGameObjectName);
                    if (newGameObjectParent)
                        newGameObject->SetParent(newGameObjectParent);
                    newGameObjectParent = nullptr;
                    hierarchyMenu = false;
                    strcpy_s(newGameObjectName,"New GameObject");
                }
            }
            ImGui::End();
        }

        if (ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_LEFT))
        {
            hierarchyMenu = false;
            selectedGameObject = nullptr;
        }
    }
    ImGui::End();
}

void Editor::MoveSelectedGameobject()
{
    if (selectedGameObject == nullptr)
        return;
    

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
        selectedGameObject->Translate(selectedGameObject->Forward() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
        selectedGameObject->Translate(-selectedGameObject->Forward() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
        selectedGameObject->Translate(selectedGameObject->Right() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
        selectedGameObject->Translate(-selectedGameObject->Right() * editorClock->GetDeltaTime());
}
