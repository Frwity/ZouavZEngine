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
#include "cereal/archives/json.hpp"
#include <iostream>
#include "System/PhysicSystem.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/SphereCollision.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"

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

    //TODO TEMP
    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    Load();
}

Engine::~Engine()
{
    //Save();

	render.Destroy();
    SoundManager::Destroy();
    PhysicSystem::Destroy();
}

void Engine::Load()
{
    MeshRenderer::defaultMesh = ResourcesManager::AddResourceMesh("CubeMesh");
    MeshRenderer::defaultMesh->CreateQuad();
    MeshRenderer::defaultShader = ResourcesManager::AddResourceShader("BlinnPhongShader", "resources/BlinnPhongShader.vs", "resources/BlinnPhongShader.fs");
    MeshRenderer::defaultTexture = ResourcesManager::AddResourceTexture("White", "resources/white.png");

    Texture::errorTexture = ResourcesManager::AddResourceTexture("Error", "resources/error.jpg");


    Sound* sound = ResourcesManager::AddResourceSound("TestSon", "resources/Test.wav");
    Mesh* mesh = ResourcesManager::AddResourceMesh("Skull Mesh", "resources/Skull.obj");
    Texture* texture = ResourcesManager::AddResourceTexture("Skull Tex", "resources/skull.jpg");


    ResourcesManager::AddResourceShader("TerrainShader", "resources/TerrainShader.vs", "resources/TerrainShader.fs");

    ResourcesManager::AddResourceTexture("Water", "resources/Water.png");
    ResourcesManager::AddResourceTexture("SandyGrass", "resources/SandyGrass.png");
    ResourcesManager::AddResourceTexture("Grass", "resources/Grass.png");
    ResourcesManager::AddResourceTexture("Rocks", "resources/Rocks.png");
    ResourcesManager::AddResourceTexture("Snow", "resources/Snow.png");

    GameObject* light = GameObject::CreateGameObject("Light");
    light->AddComponent<Light>(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::DIRECTIONAL);
    
    GameObject* soundSkull = GameObject::CreateGameObject("SoundSkull");
    soundSkull->AddComponent<MeshRenderer>(mesh, texture, MeshRenderer::defaultShader);
    soundSkull->AddComponent<AudioBroadcaster>(sound);
    soundSkull->AddComponent<Move>();

    GameObject* player = GameObject::CreateGameObject("Player");
    player->AddComponent<MeshRenderer>(mesh, texture, MeshRenderer::defaultShader);
    player->AddComponent<AudioListener>();
    player->AddComponent<Player>();
    player->AddComponent<Camera>(render.width, render.height)->SetMainCamera();
    player->AddComponent<RigidStatic>();
    player->AddComponent<SphereCollision>();

    GameObject* test = GameObject::CreateGameObject("test");
    test->localPosition = { 0.0f, 5.0f, 0.0f };
    test->AddComponent<MeshRenderer>(mesh, shader, texture);
    test->AddComponent<SphereCollision>();
    test->AddComponent<RigidBody>();

}

void Engine::Save()
{
    scene.Save();
}

void Engine::Update()
{
    ScriptSystem::Begin();

    Terrain terrain;
    terrain.Generate(Scene::GetCurrentScene()->GetWorld().GetChildren().at(2));
    terrain.Update();

    while (!render.Stop())
    {
        GameObject::DestroyGameObjectIfNeedTo();

        scene.GetWorld().UpdateTransform(Mat4::identity);

        TimeManager::Update();
        InputManager::Update();

        editor.NewFrame();

        ImGui::ShowDemoWindow();

        if (editor.GetState() == EDITOR_STATE::PLAYING)
        {
            SoundManager::Update();
            ScriptSystem::FixedUpdate();
            ScriptSystem::Update();
            terrain.Update();
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
        editor.DisplayProject();
        editor.MoveSelectedGameobject();

        terrain.DisplayOptionWindow();

        render.BindSceneFBO();
       
        terrain.Draw(*SceneCamera::GetSceneCamera());
        scene.Draw(*SceneCamera::GetSceneCamera());

        render.BindGameFBO();

        if (Camera::GetMainCamera())
        {
            terrain.Draw(*Camera::GetMainCamera());
            scene.Draw(*Camera::GetMainCamera());
        }

        render.BindMainFBO();
        
        editor.Update();
        render.Update();
    }
}
