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
#include "Component/AudioBroadcaster.hpp"
#include "Component/AudioListener.hpp"
#include "System/TimeManager.hpp"
#include "System/Terrain.hpp"
#include "System/InputManager.hpp"
#include "System/ScriptSystem.hpp"
#include "System/Engine.hpp"
#include "System/SoundManager.hpp"
#include "Rendering/Framebuffer.hpp"
#include "Game/Move.hpp"
#include "Game/Player.hpp"
#include "Sound.hpp"
#include <iostream>


Engine::Engine()
{
    render.Init(1300, 800);

    InputManager::SetWindow(render.window);
    InputManager::InitMouseButtons();
    InputManager::InitKeys();
    SoundManager::Init();

    //TEMP
    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);
    sceneCamera = SceneCamera(render.width, render.height);
    sceneCamera.SetSceneCamera();

    Load();
}

Engine::~Engine()
{
	render.Destroy();
    SoundManager::Destroy();
}

void Engine::Load()
{
    Shader* shader = static_cast<Shader*>(ResourcesManager::AddResource<Shader>("BlinnPhongShader", "resources/BlinnPhongShader.vs", "resources/BlinnPhongShader.fs"));
    ResourcesManager::AddResource<Shader>("TerrainShader", "resources/TerrainShader.vs", "resources/TerrainShader.fs");
    Sound* sound = static_cast<Sound*>(ResourcesManager::AddResource<Sound>("TestSon", "resources/Test.wav"));
    Mesh* mesh = static_cast<Mesh*>(ResourcesManager::AddResource<Mesh>("Skull Mesh", "resources/Skull.obj"));
    Texture* texture = static_cast<Texture*>(ResourcesManager::AddResource<Texture>("Skull Tex", "resources/skull.jpg"));
    GameObject* light = GameObject::CreateGameObject();

    light->AddComponent<Light>(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::Directional);
    scene.lights.push_back(light->GetComponent<Light>());

    GameObject* soundSkull = GameObject::CreateGameObject();
    soundSkull->AddComponent<MeshRenderer>(mesh, shader, texture);
    soundSkull->AddComponent<AudioBroadcaster>(sound);
    soundSkull->AddComponent<Move>();

    GameObject* player = GameObject::CreateGameObject();
    player->AddComponent<MeshRenderer>(mesh, shader, texture);
    player->AddComponent<AudioListener>();
    player->AddComponent<Player>();
    player->AddComponent<Camera>(render.width, render.height)->SetMainCamera();

    GameObject* skullX = GameObject::CreateGameObject();
    skullX->AddComponent<MeshRenderer>(mesh, shader, texture);
    skullX->position = Vec3::Right();
    GameObject* skullY = GameObject::CreateGameObject();
    skullY->AddComponent<MeshRenderer>(mesh, shader, texture);
    skullY->position = Vec3::Up();
    GameObject* skullZ = GameObject::CreateGameObject();
    skullZ->AddComponent<MeshRenderer>(mesh, shader, texture);
    skullZ->position = Vec3::Forward();
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
        SoundManager::Update();

        sceneCamera.Update(editor.isKeyboardEnable);

        editor.NewFrame();
        ScriptSystem::FixedUpdate();
        ScriptSystem::Update();
        
        //TODO call single editor function
        editor.DisplayMainWindow();
        editor.DisplaySceneWindow(render, render.sceneFramebuffer);
        editor.DisplayInspector();
        editor.DisplayConsoleWindow();
        editor.DisplayHierarchy();
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
