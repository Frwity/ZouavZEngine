#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "System/ResourcesManager.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/Light.hpp"
#include "System/TimeManager.hpp"
#include "System/Terrain.hpp"
#include "System/InputManager.hpp"
#include "System/ScriptSystem.hpp"
#include "System/Engine.hpp"
#include "Rendering/Framebuffer.hpp"
#include "Game/Player.hpp"
#include <iostream>


Engine::Engine()
{
    render.Init(1300, 800);

    InputManager::SetWindow(render.window);
    InputManager::InitMouseButtons();

    //TEMP
    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    sceneCamera = SceneCamera(Vec2((float)startCursorX, (float)startCursorY), render.width, render.height);
    sceneCamera.SetSceneCamera();

    Load();
}

Engine::~Engine()
{
	render.Destroy();
}

void Engine::Load()
{
    Shader* shader = static_cast<Shader*>(ResourcesManager::AddResource<Shader>("BlinnPhongShader", "resources/BlinnPhongShader.vs", "resources/BlinnPhongShader.fs"));
    ResourcesManager::AddResource<Shader>("TerrainShader", "resources/TerrainShader.vs", "resources/TerrainShader.fs");

    GameObject* light = GameObject::CreateGameObject();

    light->AddComponent<Light>(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::Directional);
    scene.lights.push_back(light->GetComponent<Light>());
}

void Engine::Update()
{
    ScriptSystem::Begin();

    Terrain terrain;
    terrain.Generate();

    while (!render.Stop())
    {
        TimeManager::Update();
        InputManager::Update();

        sceneCamera.Update(editor.isKeyboardEnable);

        editor.NewFrame();

        ScriptSystem::FixedUpdate();
        ScriptSystem::Update();

        editor.DisplayMainWindow();
        editor.DisplaySceneWindow(render, render.sceneFramebuffer);
        editor.DisplayInspector();
        editor.DisplayConsoleWindow();
        editor.DisplayGameWindow(render, render.gameFramebuffer);

        terrain.DisplayOptionWindow();

        render.BindSceneFBO();
       
        terrain.Draw(scene.lights, *SceneCamera::GetSceneCamera());
        scene.Draw(*SceneCamera::GetSceneCamera());

        render.BindGameFBO();

        terrain.Draw(scene.lights, *Camera::GetMainCamera());
        scene.Draw(*Camera::GetMainCamera());

        render.BindMainFBO();
        
        editor.Update();
        render.Update();
    }
}
