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
#include "Component/FontComponent.hpp"
#include "Component/Light.hpp"
#include "Component/AudioBroadcaster.hpp"
#include "Component/AudioListener.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/CapsuleCollision.hpp"
#include "Component/SphereCollision.hpp"
#include "Component/Plane.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "Component/Skybox.hpp"
#include "Game/Player.hpp"
#include "System/InputManager.hpp"
#include "System/Debug.hpp"
#include "Scene.hpp"
#include "System/TimeManager.hpp"
#include "System/Engine.hpp"
#include "System/Editor.hpp"
#include "System/ResourcesManager.hpp"

#include "Game/Player.hpp"
#include "Game/Generato.hpp"

bool newFolderWindow = false;
bool newFileWindow = false;
bool newClassWindow = false;
bool newSceneWindow = false;
bool newSceneWindowWarning = false;

std::string actualFolder = ".";
bool hierarchyMenu = false;
ImVec2 hierarchyMenuPos = { 0.0f, 0.0f };
char newGameObjectName[256] = "New GameObject";
GameObject* newGameObjectParent = nullptr;
GameObject* gameObjectInspector = nullptr;
std::string currentProjectFolder = "Project";
std::string currentMovingProjectFile;
ImVec2 projectNewFolderPos = { 0.0f, 0.0f };
bool projectNewFolder = false;

bool maximizeOnPlay = false;
bool sceneFocused = false;

bool consoleText = true;
bool consoleWarning = true;
bool consoleError = true;

static ImGuiID dockspaceID = 1;

Editor::Editor(class Engine& _engine)
    : engine(_engine), isKeyboardEnable(false)
{}

void InitImGuiStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.315f, 0.315f, 0.315f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.63f, 0.63f, 0.63f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.63f, 0.63f, 0.63f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.343f, 0.343f, 0.343f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.7f, 0.7f, 0.7f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.45f, 0.45f, 0.45f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.5f, 0.5f, 0.5f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.8f, 0.8f, 0.8f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.85f, 0.85f, 0.85f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.90f, 0.90f, 0.90f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.1f, 0.1f, 0.1f, 0.8f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 0.8f);
    colors[ImGuiCol_TabActive] = ImVec4(0.5f, 0.5f, 0.5f, 0.6f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.4f, 0.4f, 0.4f, 0.4f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.6f, 0.6f, 0.6f, 0.7f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.5f, 0.5f, 0.5f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Editor::Init()
{
    sceneCamera = SceneCamera(engine.render.width, engine.render.height);
    sceneCamera.SetSceneCamera();
    imguiStyle = &ImGui::GetStyle();
    InitImGuiStyle();

    editorClock = TimeManager::CreateClock();

    editorTextures.emplace("Folder", std::make_unique<Texture>( "Folder", "resources/folder.png" ));
    editorTextures.emplace("Default Icon", std::make_unique<Texture>( "Default Icon", "resources/default.png" ));
    editorTextures.emplace(".obj", std::make_unique<Texture>( ".obj", "resources/mesh.png" ));
    editorTextures.emplace(".fbx", std::make_unique<Texture>( ".fbx", "resources/mesh.png" ));
    editorTextures.emplace(".png", std::make_unique<Texture>( ".png", "resources/texture.png" ));
    editorTextures.emplace(".jpg", std::make_unique<Texture>( ".jpg", "resources/texture.png" ));
    editorTextures.emplace(".shader", std::make_unique<Texture>( ".vs", "resources/shader.png" ));
    editorTextures.emplace(".cpp", std::make_unique<Texture>( ".cpp", "resources/script.png" ));
    editorTextures.emplace(".hpp", std::make_unique<Texture>( ".hpp", "resources/script.png" ));
    editorTextures.emplace(".wav", std::make_unique<Texture>( ".wav", "resources/sound.png" ));
    editorTextures.emplace(".zes", std::make_unique<Texture>( ".zes", "resources/save.png" ));
    editorTextures.emplace(".zepref", std::make_unique<Texture>( ".zepref", "resources/prefab.png" ));
}

void Editor::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

bool Editor::Display(Render& _render)
{
    DisplayMainWindow();
    DisplayOptionWindow();
    DisplayGameWindow(_render.gameFramebuffer);
    if (!(state == EDITOR_STATE::PLAYING && maximizeOnPlay))
    {
        DisplaySceneWindow(_render, _render.sceneFramebuffer);
        DisplayInspector();
        DisplayConsoleWindow();
        DisplayHierarchy();
        DisplayProject();
        //MoveSelectedGameobject();
        return true;
    }
    return false;
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
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
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
    if (ImGui::Button("Save"))
    {
        engine.Save();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        gameObjectInspector = nullptr;
        engine.Load();
    }
	ImGui::SameLine();

    ImGui::SetCursorPosX(200);
    
    if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
        currentGizmoOperation = ImGuizmo::TRANSLATE;
    
    ImGui::SameLine();
    
    if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
        currentGizmoOperation = ImGuizmo::ROTATE;
    
    ImGui::SameLine();
    
    if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
        currentGizmoOperation = ImGuizmo::SCALE;
   
    ImGui::SameLine();
	ImGui::SetCursorPosX(800);

	if (ImGui::Button(state == EDITOR_STATE::PAUSE ? "Continue" : "Play"))
	{
        if (state == EDITOR_STATE::EDITING) 
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
        engine.scene.Load(false);
	}
    ImGui::SameLine();
    ImGui::Checkbox("Maximize On Play", &maximizeOnPlay);

	ImGui::End();
}

void ListActualFolder(bool& windowOpened)
{
    if (ImGui::Button("<--"))
        actualFolder = actualFolder.substr(0, actualFolder.find_last_of("/\\"));

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
        static std::string folderName = "New Folder";

        ImGui::InputText("Folder Name", folderName.data(), 256);

        if (ImGui::Button("Create"))
        {
            if (_mkdir(std::string(actualFolder).append("/").append(folderName).c_str()) == 0)
            {
                std::cout << "Folder " << folderName << " created" << std::endl;
                newFolderWindow = !newFolderWindow;
                folderName = "New Folder";
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

        static std::string fileName = "New File";
        ImGui::InputText("File Name", fileName.data(), 256);

        if (ImGui::Button("Create"))
        {
            if (std::fstream(std::string(actualFolder).append("/").append(fileName).c_str()))
                std::cout << "File " << fileName << " not created" << std::endl;
            else
                if (std::ofstream(std::string(actualFolder).append("/").append(fileName).c_str()))
                {
                    std::cout << "File " << fileName << " created" << std::endl;
                    newFileWindow = !newFileWindow;
                    fileName = "New File";
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

        static std::string className = "NewClass";
        ImGui::InputText("Class Name", className.data(), 256);

        if (ImGui::Button("Create"))
            CreateNewClass(className);

        ImGui::End();
    }
}

void NewSceneWindow(Engine& engine)
{
    if (newSceneWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f));
        ImGui::Begin("New Scene", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

        static std::string sceneName = "New Scene";
        ImGui::InputText("Scene Name", sceneName.data(), 256);
        
        if (!ImGui::IsWindowHovered() && InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT))
            newSceneWindow = false;

        if (!newSceneWindowWarning && ImGui::Button("Create"))
        {
            gameObjectInspector = nullptr;
            if (!Scene::NewScene(sceneName.c_str(), false))
                newSceneWindowWarning = true;
            else
            {
                engine.LoadDefaultResources();
                sceneName = "New Scene";
                newSceneWindow = false;
            }
        }
        if (newSceneWindowWarning)
        {
            ImGui::TextWrapped("A scene with the same name already exist, are you sure you want to create it ?");
            if (ImGui::Button("Create"))
            {
                Scene::NewScene(sceneName.c_str(), true);
                newSceneWindowWarning = false;
                engine.LoadDefaultResources();
                sceneName = "New Scene";
                newSceneWindow = false;
            }
        }
        ImGui::End();
    }
}

void Editor::DisplayMenuBar()
{
    NewFolderWindow();

    NewFileWindow();

    NewClassWindow();

    NewSceneWindow(engine);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Folder", nullptr, &newFolderWindow);
            ImGui::MenuItem("New File", nullptr, &newFileWindow);
            ImGui::MenuItem("New Class", nullptr, &newClassWindow);
            ImGui::MenuItem("New Scene", nullptr, &newSceneWindow);
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

    if (sceneFocused)
        sceneCamera.Update(isKeyboardEnable, editorClock->GetDeltaTime());
}

void Editor::DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer)
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        sceneFocused = ImGui::IsWindowFocused();
        if (ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON::BUTTON_RIGHT) && !isKeyboardEnable)
        {
            ImGui::SetWindowFocus("Scene");
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
        {
            SceneCamera::GetSceneCamera()->Resize((int)windowSize.x, (int)windowSize.y);
            _framebuffer.Resize((int)windowSize.x, (int)windowSize.y);
        }

        ImGui::Image((ImTextureID)_framebuffer.getTexture(), ImVec2((float)_framebuffer.getWidth(), (float)_framebuffer.getHeight()), ImVec2(0,1), ImVec2(1,0));

        if (gameObjectInspector)
        {
            ImGuizmo::Enable(true);

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            static ImGuizmo::MODE currentGizmosMode(ImGuizmo::LOCAL);

            Mat4 matrix = Mat4::CreateTRSMatrix(gameObjectInspector->localPosition, gameObjectInspector->localRotation, gameObjectInspector->localScale);
            Mat4 viewMatrix = SceneCamera::GetSceneCamera()->GetMatrix().Reversed();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, _framebuffer.getWidth(), _framebuffer.getHeight());
            ImGuizmo::Manipulate(viewMatrix.matrix, sceneCamera.GetProjetionMatrix().matrix, currentGizmoOperation, currentGizmosMode, matrix.matrix);
            if (ImGuizmo::IsUsing)
            {
                Vec3 translation, rotation, scale;

                ImGuizmo::DecomposeMatrixToComponents(matrix.matrix, translation.xyz, rotation.xyz, scale.xyz);
                gameObjectInspector->localPosition = translation;
                gameObjectInspector->localRotation = Quaternion(rotation);
                gameObjectInspector->localScale = scale;
            }
        }
    }
    ImGui::End();
}

template <typename T>
bool ComponentButton(std::string _text, bool _onlyOne)
{
    if ((!_onlyOne || (_onlyOne && !gameObjectInspector->GetComponent<T>())) && ImGui::Button(_text.c_str()))
    {
        gameObjectInspector->AddComponent<T>();
        return true;
    }
    return false;
}

void Editor::DisplayInspector()
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        if (gameObjectInspector)
        {
            ImGui::PushItemWidth(200.0f);
            bool isActive = gameObjectInspector->isActive;
            ImGui::Checkbox(" ", &isActive);
            if (isActive != gameObjectInspector->isActive)
                isActive ? gameObjectInspector->Activate() : gameObjectInspector->Dehactivate();
            ImGui::SameLine();
            ImGui::Text("Name : ");
            ImGui::SameLine();
            std::string name = gameObjectInspector->name;
            if (ImGui::InputText("##name", name.data(), 256))
                gameObjectInspector->name = name.data();

            ImGui::SameLine();
            ImGui::Text("Tag : ");
            ImGui::SameLine();
            std::string tag = gameObjectInspector->tag;
            if (ImGui::InputText("##tag", tag.data(), 256))
                gameObjectInspector->tag = tag.data();


            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("World Position : %.3f %.3f %.3f", gameObjectInspector->WorldPosition().x, gameObjectInspector->WorldPosition().y, gameObjectInspector->WorldPosition().z);

                Vec3 worldEulerAngles = gameObjectInspector->WorldRotation().ToEuler();
                ImGui::Text("World Rotation : %.3f %.3f %.3f", worldEulerAngles.x, worldEulerAngles.y, worldEulerAngles.z);

                ImGui::Text("World Scale    : %.3f %.3f %.3f", gameObjectInspector->WorldScale().x, gameObjectInspector->WorldScale().y, gameObjectInspector->WorldScale().z);

                ImGui::Text("Local Position :");
                ImGui::SameLine(); ImGui::InputFloat3("##positionx", &gameObjectInspector->localPosition.x);
                
                static Vec3 localEulerAngles;
                localEulerAngles = gameObjectInspector->localRotation.ToEuler();

                ImGui::Text("Local Rotation :");
                ImGui::SameLine(); if (ImGui::InputFloat3("##rotation", &localEulerAngles.x, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
                    gameObjectInspector->localRotation = Quaternion(localEulerAngles);

                ImGui::Text("Local Scale    :");
                ImGui::SameLine(); ImGui::InputFloat3("##scalex", &gameObjectInspector->localScale.x);
            }
            for (std::unique_ptr<Component>& component : gameObjectInspector->components)
            {
                Component* comp = component.get();
                ImGui::PushID(comp);
                bool active = comp->isActive;
                ImGui::Checkbox("", &active);
                if (active != comp->isActive)
                    active ? comp->Activate() : comp->Dehactivate();
                ImGui::SameLine();
                if (!Component::EditorCollapsingHeader(comp->GetComponentName(), [comp]() {comp->Editor(); }))
                {
                    comp->DeleteFromGameObject();
                    ImGui::PopID();
                    break;
                }
                ImGui::PopID();
            }

            static bool addComponentWindow = false;
            
            if (addComponentWindow)
            {
                ImVec2 windowSize = ImGui::GetWindowSize();
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + windowSize.x / 4, ImGui::GetWindowPos().y + windowSize.y / 4));
                ImGui::SetNextWindowSize(ImVec2(windowSize.x / 2, windowSize.y / 2));
                if (ImGui::Begin("Add Component##window", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
                {
                    for (int i = 0; i < static_cast<int>(E_COMPONENT::NUMBER_OF_COMPONENTS); i++)
                    {
                        switch (static_cast<E_COMPONENT>(i))
                        {
                        case E_COMPONENT::AUDIO_BROADCASTER:
                            if (ComponentButton<AudioBroadcaster>("Add AudioBroadcaster", true))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::AUDIO_LISTENER:
                            if (ComponentButton<AudioListener>("Add AudioListener", true))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::LIGHT:
                            if (ComponentButton<Light>("Add Light", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::MESHRENDERER:
                            if (ComponentButton<MeshRenderer>("Add MeshRenderer", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::FONTCOMPONENT:
                            if (ComponentButton<FontComponent>("Add FontComponent", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::BOX_COLLISION:
                            if (ComponentButton<BoxCollision>("Add BoxCollision", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::CAPSULE_COLLISION:
                            if (ComponentButton<CapsuleCollision>("Add CapsuleCollision", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::SPHERE_COLLISION:
                            if (ComponentButton<SphereCollision>("Add SphereCollision", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::PLANE:
                            if (gameObjectInspector->GetComponent<RigidStatic>())
                                if (ComponentButton<Plane>("Add Plane", false))
                                    addComponentWindow = false;
                            break;
                        case E_COMPONENT::RIGID_BODY:
                            if (!gameObjectInspector->GetComponent<RigidStatic>())
                                if (ComponentButton<RigidBody>("Add RigidBody", true))
                                    addComponentWindow = false;
                            break;
                        case E_COMPONENT::RIGID_STATIC:
                            if (!gameObjectInspector->GetComponent<RigidBody>())
                                if (ComponentButton<RigidStatic>("Add RigidStatic", true))
                                    addComponentWindow = false;
                            break;
                        case E_COMPONENT::CAMERA:
                            if (ComponentButton<Camera>("Add Camera", true))
                            {
                                addComponentWindow = false;
                                gameObjectInspector->GetComponent<Camera>()->Resize(engine.render.gameFramebuffer.getWidth(), engine.render.gameFramebuffer.getHeight());
                            }
                            break;
                        case E_COMPONENT::GENERATOR:
                            if (ComponentButton<Generato>("Add Generator", false))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::SKYBOX:
                            if (ComponentButton<Skybox>("Add Skybox", true))
                                addComponentWindow = false;
                            break;
                        case E_COMPONENT::PLAYER:
                            if (ComponentButton<Player>("Add Player", true))
                            {
                                addComponentWindow = false;
                                gameObjectInspector->GetComponent<Player>()->Begin();
                            }
                            break;
                        }
                    }
                }
                if (!ImGui::IsWindowHovered() && InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT))
                    addComponentWindow = false;
                
                ImGui::End();
            }
            if (ImGui::Button("Add Component"))
                addComponentWindow = true;
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

void Editor::DisplayGameWindow(class Framebuffer& _framebuffer)
{
    if (ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();

        if ((int)windowSize.x != _framebuffer.getWidth() || (int)windowSize.y != _framebuffer.getHeight())
        {
            if (Camera::GetMainCamera())
                Camera::GetMainCamera()->Resize((int)windowSize.x, (int)windowSize.y);
            _framebuffer.Resize((int)windowSize.x, (int)windowSize.y);
        }
        ImGui::Image((ImTextureID)_framebuffer.getTexture(), ImVec2((float)_framebuffer.getWidth(), (float)_framebuffer.getHeight()), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

void Editor::DisplayProject()
{
    if (ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs))
    {
        if (ImGui::Button("Project"))
            currentProjectFolder = "Project";
        if (GetRightName(currentProjectFolder).compare("Project") != 0)
        {
            ImGui::SameLine();
            if(ImGui::Button("<--"))
                currentProjectFolder = currentProjectFolder.substr(0, currentProjectFolder.find_last_of("/\\"));
        }
        int i = 0;
        float windowWidth = ImGui::GetContentRegionAvailWidth();
        int nbButton = ((windowWidth - 20.0f) / 72.0f) - 2;

        for (const auto& entry : std::filesystem::directory_iterator(currentProjectFolder))
        {
            if (entry.path().extension().string().compare(".zesr") == 0)
                continue;

            if (i++ != 0 && nbButton > 0 && (i-1) % nbButton != 0)
                ImGui::SameLine();

            std::string currentName = GetRightName(entry.path().string());
            if (entry.is_directory())
            {
                ImGui::PushID(i);
                ImGui::BeginGroup();
                if (ImGui::ImageButton((ImTextureID)editorTextures.find("Folder")->second->textureID, ImVec2(60.0f, 60.0f), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), 12, ImVec4(100.f, 100.f, 100.f, 0.f)))
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
                ImGui::Text(currentName.size() > 11 ? (currentName.substr(0, 9) + "...").c_str() : currentName.c_str());
                ImGui::EndGroup();
                ImGui::PopID();
            }
            else
            {
                ImGui::PushID(i);
                ImGui::BeginGroup();
                auto texture = editorTextures.find(entry.path().extension().string().c_str());
                if (ImGui::ImageButton(texture != editorTextures.end() ? (ImTextureID)texture->second->textureID : (ImTextureID)editorTextures.find("Default Icon")->second->textureID, ImVec2(60.0f, 60.0f), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), 12, ImVec4(100.f, 100.f, 100.f, 0.f))
                && entry.path().extension().string().compare(".zes") == 0)
                {
                    gameObjectInspector = nullptr;
                    engine.scene.Load(entry.path().string());
                }
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    currentMovingProjectFile = entry.path().string();
                    ImGui::SetDragDropPayload("ProjectFile", &currentMovingProjectFile, sizeof(std::string));
                    ImGui::Text(currentName.c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::Text(currentName.size() > 11 ? (currentName.substr(0, 9) + "...").c_str() : currentName.c_str());
                ImGui::EndGroup();
                ImGui::PopID();
            }
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
                if (!ImGui::IsWindowHovered() && InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT))
                    projectNewFolder = false;

                static std::string newHierarchyFolderName = "New Folder";
                ImGui::InputText("##newhierarchyfoldername", newHierarchyFolderName.data(), 256);
                if (ImGui::Button("Create"))
                {
                    if (_mkdir(std::string(currentProjectFolder).append("/").append(newHierarchyFolderName).c_str()) == 0)
                        std::cout << "Folder " << newHierarchyFolderName << " created" << std::endl;
                    else
                        std::cout << "Folder " << newHierarchyFolderName << " not created" << std::endl;
                    projectNewFolder = false;
                    newHierarchyFolderName = "New Folder";
                }
            }
            ImGui::End();
        }
    }
    ImGui::End();
}

void Editor::DisplayChild(GameObject* _parent)
{
    ImGui::PushID(_parent);
    if (ImGui::TreeNodeEx(_parent->GetName().c_str(), _parent->GetChildren().size() != 0 ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
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
            ImGui::Text(_parent->GetName().c_str());

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
    ImGui::PopID();
}

void Editor::DisplayHierarchy()
{
    ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
            {
                ZASSERT(payload->DataSize == sizeof(std::string), "Error in moving file in hierarchy");
                std::string path = *(const std::string*)payload->Data;
                if (path.find(".zepref") != std::string::npos) //abcdefg
                {
                    GameObject::Instanciate(GameObject::LoadPrefab(path));
                }
            }
            ImGui::EndDragDropTarget();
        }

        DisplayChild(&Scene::GetCurrentScene()->GetWorld());

        if (!isKeyboardEnable && ImGui::IsWindowHovered() && InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_RIGHT))
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

                if (newGameObjectParent && newGameObjectParent->parent)
                {
                    if (ImGui::Button("Delete"))
                    {
                        for (GameObject* child : newGameObjectParent->children)
                        {
                            child->SetParent(newGameObjectParent->parent);
                        }
                        newGameObjectParent->SetParent(nullptr);
                        newGameObjectParent->toDestroy = true;
                        GameObject::destroyGameObject = true;
                        hierarchyMenu = false;
                        if (newGameObjectParent == gameObjectInspector)
                            gameObjectInspector = nullptr;
                        newGameObjectParent = nullptr;
                    }
                    if (ImGui::Button("CreatePrefab"))
                    {
                        newGameObjectParent->CreatePrefab();
                    }
                    if (ImGui::Button("Copy/Paste"))
                        GameObject::Instanciate(newGameObjectParent);
                }

                if (!ImGui::IsWindowHovered() && InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT))
                    hierarchyMenu = false;
            }
            ImGui::End();
        }
    }
    ImGui::End();
}

void Editor::MoveSelectedGameobject()
{
    if (gameObjectInspector == nullptr || state == EDITOR_STATE::PLAYING || state == EDITOR_STATE::PAUSE)
        return;

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
        gameObjectInspector->Translate(-gameObjectInspector->Forward() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
        gameObjectInspector->Translate(gameObjectInspector->Forward() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
        gameObjectInspector->Translate(gameObjectInspector->Right() * editorClock->GetDeltaTime());

    if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
        gameObjectInspector->Translate(-gameObjectInspector->Right() * editorClock->GetDeltaTime());
}