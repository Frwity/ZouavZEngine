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
#include "System/PhysicSystem.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/SphereCollision.hpp"

#define EDITOR

Engine::Engine()
    : editor(*this)
{
    render.Init(1300, 800);

    InputManager::SetWindow(render.window);
    InputManager::InitMouseButtons();
    InputManager::InitKeys();

    SoundManager::Init();
    PhysicSystem::Init();

    TimeManager::Init();

#ifdef EDITOR
    editor.Init();
#endif

    //TEMP
    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    Load();
}

Engine::~Engine()
{
	render.Destroy();
    SoundManager::Destroy();
    PhysicSystem::Destroy();
}

void Engine::Load()
{
    Shader* shader = static_cast<Shader*>(ResourcesManager::AddResource<Shader>("BlinnPhongShader", "resources/BlinnPhongShader.vs", "resources/BlinnPhongShader.fs"));
    ResourcesManager::AddResource<Shader>("TerrainShader", "resources/TerrainShader.vs", "resources/TerrainShader.fs");
    Sound* sound = static_cast<Sound*>(ResourcesManager::AddResource<Sound>("TestSon", "resources/Test.wav"));
    Mesh* mesh = static_cast<Mesh*>(ResourcesManager::AddResource<Mesh>("Skull Mesh", "resources/Skull.obj"));
    Texture* texture = static_cast<Texture*>(ResourcesManager::AddResource<Texture>("Skull Tex", "resources/skull.jpg"));
    GameObject* light = GameObject::CreateGameObject("Light");

    light->AddComponent<Light>(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::Directional);
    scene.lights.push_back(light->GetComponent<Light>());

    GameObject* soundSkull = GameObject::CreateGameObject("SoundSkull");
    soundSkull->AddComponent<MeshRenderer>(mesh, shader, texture);
    soundSkull->AddComponent<AudioBroadcaster>(sound);
    soundSkull->AddComponent<Move>();

    GameObject* player = GameObject::CreateGameObject("Player");
    player->AddComponent<MeshRenderer>(mesh, shader, texture);
    player->AddComponent<AudioListener>();
    player->AddComponent<Player>();
    player->AddComponent<Camera>(render.width, render.height)->SetMainCamera();
    player->AddComponent<BoxCollision>();

    GameObject* test = GameObject::CreateGameObject("test");
    test->localPosition = { 0.0f, 2.0f, 0.0f };
    test->AddComponent<MeshRenderer>(mesh, shader, texture);
    test->AddComponent<SphereCollision>();
}

void Engine::Update()
{
    ScriptSystem::Begin();

    Terrain terrain;
    terrain.Generate();

    while (!render.Stop())
    {
        scene.GetWorld().UpdateTransform(Mat4::identity);

        TimeManager::Update();
        InputManager::Update();

        editor.NewFrame();

        if (editor.GetState() == EDITOR_STATE::PLAYING)
        {
            SoundManager::Update();
            ScriptSystem::FixedUpdate();
            ScriptSystem::Update();
            scene.SimulatePhyics();
        }

        //TODO call single editor function
        editor.DisplayMainWindow();
        editor.DisplayOptionWindow();
        editor.DisplaySceneWindow(render, render.sceneFramebuffer);
        editor.DisplayInspector();
        editor.DisplayConsoleWindow();
        editor.DisplayHierarchy();
        editor.DisplayGameWindow(render, render.gameFramebuffer);
        editor.MoveSelectedGameobject();

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
