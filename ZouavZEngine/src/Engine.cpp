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

    ResourcesManager::InitDefaultResources();

#ifdef EDITOR
    editor.Init();
#endif

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    LoadDefaultResources();
    TempLoad();
}

Engine::~Engine()
{
    //Save();

	render.Destroy();
    SoundManager::Destroy();
    PhysicSystem::Destroy();
}

void Engine::LoadDefaultResources()
{
    ResourcesManager::AddResourceShader("TerrainShader", "resources/TerrainShader.vs", "resources/TerrainShader.fs");
    Texture::errorTexture = ResourcesManager::AddResourceTexture("Error", "resources/error.jpg");
    ResourcesManager::AddResourceTexture("Folder", "resources/folder.png");
    ResourcesManager::AddResourceTexture("Default Icon", "resources/default.png");
    ResourcesManager::AddResourceTexture(".obj", "resources/mesh.png");
    ResourcesManager::AddResourceTexture(".fbx", "resources/mesh.png");
    ResourcesManager::AddResourceTexture(".png", "resources/texture.png");
    ResourcesManager::AddResourceTexture(".jpg", "resources/texture.png");
    ResourcesManager::AddResourceTexture(".vs", "resources/shader.png");
    ResourcesManager::AddResourceTexture(".fs", "resources/shader.png");
    ResourcesManager::AddResourceTexture(".cpp", "resources/script.png");
    ResourcesManager::AddResourceTexture(".hpp", "resources/script.png");
    ResourcesManager::AddResourceTexture(".wav", "resources/sound.png");
}

void Engine::TempLoad()
{
    // other resource
    Sound* sound = ResourcesManager::AddResourceSound("TestSon", "Project/sounds/Test.wav");
    Mesh* mesh = ResourcesManager::AddResourceMesh("Skull Mesh", "Project/meshes/Skull.obj");

    Texture* texture = ResourcesManager::AddResourceTexture("Skull Tex", "Project/textures/skull.jpg");

    ResourcesManager::AddResourceTexture("Water", "Project/textures/Water.png");
    ResourcesManager::AddResourceTexture("SandyGrass", "Project/textures/SandyGrass.png");
    ResourcesManager::AddResourceTexture("Grass", "Project/textures/Grass.png");
    ResourcesManager::AddResourceTexture("Rocks", "Project/textures/Rocks.png");
    ResourcesManager::AddResourceTexture("Snow", "Project/textures/Snow.png");
    

    // coded scene
    GameObject* light = GameObject::CreateGameObject("Light");
    light->AddComponent<Light>(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::DIRECTIONAL);

    GameObject* soundSkull = GameObject::CreateGameObject("SoundSkull");
    soundSkull->AddComponent<MeshRenderer>(mesh, texture, ResourcesManager::GetResource<Shader>("Default"));
    soundSkull->AddComponent<AudioBroadcaster>(sound);
    soundSkull->AddComponent<Move>();

    GameObject* player = GameObject::CreateGameObject("Player");
    player->tag = "Player";
    player->AddComponent<MeshRenderer>(mesh, texture, ResourcesManager::GetResource<Shader>("Default"));

    player->AddComponent<AudioListener>();
    player->AddComponent<Player>();
    player->AddComponent<Camera>(render.width, render.height)->SetMainCamera();
    //player->AddComponent<SphereCollision>();
    //player->AddComponent<RigidBody>();
    //player = GameObject::CreateGameObject("oui");
    //player->AddComponent<MeshRenderer>(ResourcesManager::GetResource<Mesh>("Default"), texture, ResourcesManager::GetResource<Shader>("Default"));
    //player->AddComponent<BoxCollision>();
    //player->AddComponent<Player>();
    //player->AddComponent<RigidBody>();
    //player = GameObject::CreateGameObject("oui");
    //player->AddComponent<MeshRenderer>(ResourcesManager::GetResource<Mesh>("Default"), texture, ResourcesManager::GetResource<Shader>("Default"));
    //player->AddComponent<BoxCollision>();
    //player->AddComponent<Player>();
    //player->AddComponent<RigidBody>();
    //player = GameObject::CreateGameObject("oui");
    //player->AddComponent<MeshRenderer>(ResourcesManager::GetResource<Mesh>("Default"), texture, ResourcesManager::GetResource<Shader>("Default"));
    //player->AddComponent<BoxCollision>();
    //player->AddComponent<Player>();
    //player->AddComponent<RigidBody>();
    //player = GameObject::CreateGameObject("oui");
    //player->AddComponent<MeshRenderer>(ResourcesManager::GetResource<Mesh>("Default"), texture, ResourcesManager::GetResource<Shader>("Default"));
    //player->AddComponent<BoxCollision>();
    //player->AddComponent<Player>();
    //player->AddComponent<RigidBody>();
    //GameObject* test = GameObject::CreateGameObject("test");
    //test->localPosition = { 0.0f, 5.0f, 0.0f };
    //test->AddComponent<MeshRenderer>(mesh, texture, ResourcesManager::GetResource<Shader>("Default"));
    //test->AddComponent<SphereCollision>();
    //test->AddComponent<RigidBody>();
}

void Engine::Load()
{
    scene.Load();
}

void Engine::Save()
{
    scene.Save();
}

void Engine::Update()
{
    ScriptSystem::Begin();

    while (!render.Stop())
    {
        TimeManager::Update();
        InputManager::Update();

        editor.NewFrame();

        ImGui::ShowDemoWindow();

        if (editor.GetState() == EDITOR_STATE::PLAYING)
        {
            SoundManager::Update();
            ScriptSystem::FixedUpdate();
            ScriptSystem::Update();
            scene.GetWorld().UpdateTransform(Mat4::identity);
            scene.Update();
        }
        else
            scene.GetWorld().UpdateTransform(Mat4::identity);
       
        GameObject::DestroyGameObjectIfNeedTo();

        editor.Display(render);

        scene.DisplayTerrainOptionWindow();

        render.BindSceneFBO();
       
        scene.Draw(*SceneCamera::GetSceneCamera());

        render.BindGameFBO();
        
        if (Camera::GetMainCamera())
            scene.Draw(*Camera::GetMainCamera());
        
        render.BindMainFBO();
        
        editor.Update();
        render.Update();
    }
}
