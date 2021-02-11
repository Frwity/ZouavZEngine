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
    float cameraSpeed = TimeManager::GetDeltaTime() * camera.Speed() + camera.Speed() * sprint * 0.2f;

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
	render.Init(1400, 900);

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
    Shader* shader = static_cast<Shader*>(ResourcesManager::AddResource<Shader>("shader", "resources/shader.vs", "resources/shader.fs"));
    Mesh* skullMesh = static_cast<Mesh*>(ResourcesManager::AddResource<Mesh>("Skull Mesh", "resources/Skull.obj"));
    Mesh* innMesh = static_cast<Mesh*>(ResourcesManager::AddResource<Mesh>("Inn Mesh", "resources/fantasy_game_inn.obj"));
    Texture* skullTexture = static_cast<Texture*>(ResourcesManager::AddResource<Texture>("Skull Texture", "resources/skull.jpg"));
    Texture* innTexture = static_cast<Texture*>(ResourcesManager::AddResource<Texture>("Inn Texture", "resources/fantasy_game_inn_diffuse.png"));
    GameObject* inn = GameObject::CreateGameObject();
    GameObject* skull = GameObject::CreateGameObject();
    inn->AddChild(skull);
    GameObject* skull2 = GameObject::CreateGameObject();
    skull2->SetParent(skull);

    GameObject* light1 = GameObject::CreateGameObject();
    GameObject* light2 = GameObject::CreateGameObject();
    GameObject* light3 = GameObject::CreateGameObject();

    light1->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);
    light2->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);
    light3->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);

    light1->scale = { 0.2f, 0.2f, 0.2f };
    light2->scale = { 0.2f, 0.2f, 0.2f };
    light3->scale = { 0.2f, 0.2f, 0.2f };
    light3->position = { -5.0f, 0.0f, 0.0f };

    light1->AddComponent<Light>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::Spot);
    light2->AddComponent<Light>(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::Directional);
    light3->AddComponent<Light>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.5f, 0.0f), Vec3(0.0f, 0.5f, 0.0f), Vec3(1.0f, 0.01f, 0.001f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.9f, 0.8f), E_LIGHT_TYPE::Point);

    inn->AddComponent<MeshRenderer>(innMesh, shader, innTexture);
    skull->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);
    skull2->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);

    skull->AddComponent<Player>();

    skull2->position.x = -5;

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    scene.lights.push_back(light1->GetComponent<Light>());
    scene.lights.push_back(light2->GetComponent<Light>());
    scene.lights.push_back(light3->GetComponent<Light>());
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
    while (!render.Stop())
    {
        TimeManager::Update();

        {
            glfwPollEvents();

            InputManager(render.window, camera);
        }

        ScriptSystem::FixedUpdate();
        ScriptSystem::Update();

        {
            editor.NewFrame();

            editor.DisplayMainWindow();

            if (ImGui::Begin("test1", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::End();
            }
        
            if (ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoScrollbar))
            {
                ImVec2 windowSize = ImGui::GetWindowSize();

                if ((int)windowSize.x != frameBuffer.getWidth() || (int)windowSize.y != frameBuffer.getHeight())
                    frameBuffer.Resize(windowSize.x, windowSize.y);

                ImGui::Image((ImTextureID)frameBuffer.getTexture(), ImVec2(frameBuffer.getWidth(), frameBuffer.getHeight()), ImVec2(0, 1), ImVec2(1, 0));
                ImGui::End();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getId());

            glViewport(0, 0, frameBuffer.getWidth(), frameBuffer.getHeight());
            render.Clear();
        }

        {
            scene.Draw();

            glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
            glViewport(0, 0, (int)render.width, (int)render.height);
            render.Clear();
        }
        
        editor.Update();
        render.Update();
    }
}
