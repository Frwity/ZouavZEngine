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
#include "Component/Light.hpp"
#include "Component/AudioBroadcaster.hpp"
#include "Component/AudioListener.hpp"
#include "System/InputManager.hpp"
#include "System/Debug.hpp"
#include "Scene.hpp"
#include "System/TimeManager.hpp"
#include "System/Engine.hpp"
#include "System/Editor.hpp"
#include "System/ResourcesManager.hpp"

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
GameObject* gameObjectInspector = nullptr;
std::string currentProjectFolder = "resources";
std::string currentMovingProjectFile;
ImVec2 projectNewFolderPos = { 0.0f, 0.0f };
bool projectNewFolder = false;


bool consoleText = true;
bool consoleWarning = true;
bool consoleError = true;

static ImGuiID dockspaceID = 1;

Editor::Editor(class Engine& _engine)
    : engine(_engine), isKeyboardEnable(false)
{}

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
	ImGui::Begin("Option", NULL,  //ImGuiWindowFlags_NoMove
                                //| ImGuiWindowFlags_NoDocking
                                 ImGuiWindowFlags_NoNav
                                | ImGuiWindowFlags_NoBackground
		                        | ImGuiWindowFlags_NoScrollWithMouse
		                        | ImGuiWindowFlags_NoTitleBar
		                        | ImGuiWindowFlags_NoScrollbar
                                | ImGuiWindowFlags_NoCollapse
		                        | ImGuiWindowFlags_NoResize
    );
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        engine.Save();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        engine.Load();
    }
	ImGui::SameLine();
	ImGui::SetCursorPosX(800);

	if (ImGui::Button("Play"))
	{
        engine.Save();
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
        gameObjectInspector = nullptr;
        imguiStyle->Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
        engine.Load();
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
            ImGui::MenuItem("Load", nullptr);
            ImGui::MenuItem("Save", nullptr);
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
            ImGui::PushItemWidth(200.0f);
            ImGui::InputText("##name", gameObjectInspector->name.data(), 256);

            ImGui::Text("World Position : %.3f %.3f %.3f", gameObjectInspector->WorldPosition().x, gameObjectInspector->WorldPosition().y, gameObjectInspector->WorldPosition().z);
            
            Vec3 worldEulerAngles = gameObjectInspector->WorldRotation().ToEuler();
            ImGui::Text("World Rotation : %.3f %.3f %.3f", worldEulerAngles.x, worldEulerAngles.y, worldEulerAngles.z);
            
            ImGui::Text("World Scale    : %.3f %.3f %.3f", gameObjectInspector->WorldScale().x, gameObjectInspector->WorldScale().y, gameObjectInspector->WorldScale().z);

            ImGui::Text("Local Position :");
            ImGui::SameLine(); ImGui::InputFloat3("##positionx", &gameObjectInspector->localPosition.x);

            static Vec3 localEulerAngles = gameObjectInspector->localRotation.ToEuler();

            ImGui::Text("Local Rotation :");
            ImGui::SameLine(); if (ImGui::InputFloat3("##rotation", &localEulerAngles.x))
                gameObjectInspector->localRotation = Quaternion(localEulerAngles);

            ImGui::Text("Local Scale    :");
            ImGui::SameLine(); ImGui::InputFloat3("##scalex", &gameObjectInspector->localScale.x);

            for (std::unique_ptr<Component>& component : gameObjectInspector->components)
            {
                component->Editor();
            }

            for (int i = 0; i < static_cast<int>(E_COMPONENT::NUMBER_OF_COMPONENTS); i++)
            {
                switch (i)
                {
                    case static_cast<int>(E_COMPONENT::AUDIO_BROADCASTER) :
                        if (!gameObjectInspector->GetComponent<AudioBroadcaster>())
                            if (ImGui::Button("Add AudioBroadcaster"))
                                gameObjectInspector->AddComponent<AudioBroadcaster>();
                        break;

                    case static_cast<int>(E_COMPONENT::AUDIO_LISTENER) :
                        if (!gameObjectInspector->GetComponent<AudioListener>())
                            if (ImGui::Button("Add AudioListener"))
                                gameObjectInspector->AddComponent<AudioListener>();
                        break;

                    case static_cast<int>(E_COMPONENT::LIGHT) :
                        if (!gameObjectInspector->GetComponent<Light>())
                            if (ImGui::Button("Add Light"))
                                gameObjectInspector->AddComponent<Light>();
                        break;

                    case static_cast<int>(E_COMPONENT::MESHRENDERER) :
                        if (!gameObjectInspector->GetComponent<MeshRenderer>())
                            if (ImGui::Button("Add MeshRenderer"))
                                gameObjectInspector->AddComponent<MeshRenderer>();
                        break;
                }
            }
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

void Editor::DisplayProject()
{
    if (ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        if (ImGui::Button("resources"))
            currentProjectFolder = "resources";

        int i = 0;
        float windowWidth = ImGui::GetWindowWidth();
        
        for (const auto& entry : std::filesystem::directory_iterator(currentProjectFolder))
        {
            std::string currentName = GetRightName(entry.path().string());
            if (entry.is_directory())
            {
                ImGui::PushID(i);
                if (ImGui::ButtonEx(currentName.c_str(), ImVec2(windowWidth / 3.0f - windowWidth / 100.0f, 60.0f), ImGuiButtonFlags_PressedOnDoubleClick))
                {
                    currentProjectFolder.append("/").append(currentName);
                }
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    currentMovingProjectFile = entry.path().string();
                    ImGui::SetDragDropPayload("ProjectFile", &currentMovingProjectFile, sizeof(std::string));
                    ImGui::Text(currentName.c_str());
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
                    {
                        ZASSERT(payload->DataSize == sizeof(std::string), "Error in project manager");
                        std::string name = *(const std::string*)payload->Data;
                        std::string newName = name.substr(0, name.size() - GetRightName(name).size()).append(currentName).append("/").append(GetRightName(name));
                        std::rename(name.c_str(), newName.c_str());
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }
            else
            {
                ImGui::PushID(i);
                ImGui::Button(currentName.c_str(), ImVec2(windowWidth / 3.0f - windowWidth / 100.0f, 60.0f));
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    currentMovingProjectFile = entry.path().string();
                    ImGui::SetDragDropPayload("ProjectFile", &currentMovingProjectFile, sizeof(std::string));
                    ImGui::Text(currentName.c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::PopID();
            }
            if (++i % 3 != 0)
                ImGui::SameLine();
        }
    
        if (!isKeyboardEnable && ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_RIGHT))
        {
            projectNewFolderPos = ImGui::GetMousePos();
            projectNewFolder = true;
        }

        if (projectNewFolder)
        {
            ImGui::SetNextWindowPos(projectNewFolderPos);
            if (ImGui::Begin("New Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
            {
                static char newHierarchyFolderName[256] = "New Folder";
                ImGui::InputText("##newhierarchyfoldername", newHierarchyFolderName, 256);
                if (ImGui::Button("Create"))
                {
                    if (_mkdir(std::string(currentProjectFolder).append("/").append(newHierarchyFolderName).c_str()) == 0)
                        std::cout << "Folder " << newHierarchyFolderName << " created" << std::endl;
                    else
                        std::cout << "Folder " << newHierarchyFolderName << " not created" << std::endl;
                    projectNewFolder = !projectNewFolder;
                }
            }
            ImGui::End();
        }
    }
    ImGui::End();
}

void DisplayChild(GameObject* _parent)
{
    if (ImGui::TreeNodeEx(_parent->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
    {
        if (ImGui::IsItemHovered())
        {
            if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_RIGHT))
            {
                hierarchyMenuPos = ImGui::GetMousePos();
                hierarchyMenu = true;
                newGameObjectParent = _parent;
            }

            if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT))
                gameObjectInspector = _parent;
            
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                SceneCamera::GetSceneCamera()->SetPosition(_parent->WorldPosition());
            }
        }


        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("Gameobject Hierarchy", &_parent, sizeof(_parent));
            ImGui::Text(_parent->name.c_str());

            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Gameobject Hierarchy"))
            {
                GameObject* gameObject = * (GameObject**)(payload->Data);
                if (!_parent->IsChildOf(gameObject))
                    gameObject->SetParent(_parent);
            }
            ImGui::EndDragDropTarget();
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

        if (!isKeyboardEnable && ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_RIGHT))
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
                if (ImGui::Button("Delete") && newGameObjectParent->parent)
                {
                    for (GameObject* child : newGameObjectParent->children)
                    {
                        child->SetParent(newGameObjectParent->parent);
                    }
                    newGameObjectParent->SetParent(nullptr);
                    newGameObjectParent->toDestroy = true;
                    GameObject::destroyGameObject = true;
                }
            }
            ImGui::End();
        }

        if (ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_LEFT))
            hierarchyMenu = false;
    }
    ImGui::End();
}

void Editor::MoveSelectedGameobject()
{
    if (gameObjectInspector == nullptr)
        return;

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
        gameObjectInspector->Translate(gameObjectInspector->Forward() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
        gameObjectInspector->Translate(-gameObjectInspector->Forward() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
        gameObjectInspector->Translate(gameObjectInspector->Right() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
        gameObjectInspector->Translate(-gameObjectInspector->Right() * editorClock->GetDeltaTime());
}
