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

void InputManager(GLFWwindow* window, Camera& camera)
{
    InputManager::UpdateMouseButtons();
    InputManager::UpdateKeys();

    if (InputManager::GetKeyPressed(E_KEYS::ESCAPE))
        glfwSetWindowShouldClose(window, true);

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    camera.UpdateRotation({ (float)cursorX, (float)cursorY });

    bool sprint = InputManager::GetKeyPressed(E_KEYS::LCTRL);
    float cameraSpeed = TimeManager::GetDeltaTime() * camera.Speed() + camera.Speed() * sprint * 1.2f;

    if (InputManager::GetKeyPressed(E_KEYS::W))
        camera.MoveTo({ 0.0f, 0.0f, -cameraSpeed });

    if (InputManager::GetKeyPressed(E_KEYS::S))
        camera.MoveTo({ 0.0f, 0.0f, cameraSpeed });

    if (InputManager::GetKeyPressed(E_KEYS::D))
        camera.MoveTo({ cameraSpeed, 0.0f, 0.0f });

    if (InputManager::GetKeyPressed(E_KEYS::A))
        camera.MoveTo({ -cameraSpeed, 0.0f, 0.0f });

    if (InputManager::GetKeyPressed(E_KEYS::SPACEBAR))
        camera.MoveTo({ 0.0f, cameraSpeed, 0.0f });

    if (InputManager::GetKeyPressed(E_KEYS::LSHIFT))
        camera.MoveTo({ 0.0f, -cameraSpeed, 0.0f });
}


Engine::Engine()
{
    render.Init(1920, 1080);

    InputManager::SetWindow(render.window);
    InputManager::InitMouseButtons();

    //TEMP
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);
    camera = Camera(Vec2((float)startCursorX, (float)startCursorY), render.width, render.height);
    camera.SetMainCamera();
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

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);
}

void Engine::Update()
{
    float translation = 0.0f;
    ScriptSystem::Begin();

    int previousFramebuffer;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

    Framebuffer frameBuffer;
    frameBuffer.Generate(400, 400, GL_RGBA, GL_UNSIGNED_BYTE);
    
    int sceneWidth, sceneHeight = 0;

    bool show = false;

    Terrain terrain;

    terrain.Generate();

    camera.MoveTo({ 650, -150, 500 });

    while (!render.Stop())
    {

        TimeManager::Update();

        {
            glfwPollEvents();

            InputManager(render.window, camera);
        }

        editor.NewFrame();

        ScriptSystem::FixedUpdate();
        ScriptSystem::Update();

        editor.DisplayMainWindow();

        editor.DisplaySceneWindow(render, frameBuffer);

        terrain.DisplayOptionWindow();

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getId());
        glViewport(0, 0, frameBuffer.getWidth(), frameBuffer.getHeight());
        render.Clear();
       
        terrain.Draw(scene.lights);

        scene.Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
        glViewport(0, 0, (int)render.width, (int)render.height);
        render.Clear();
        
        editor.Update();
        render.Update();
    }
}
