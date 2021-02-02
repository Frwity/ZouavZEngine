#include "Maths/Mat4.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Resource.hpp"
#include "Rendering/Render.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/MeshRenderer.hpp"
#include "Component/Transform.hpp"
#include "Scene.hpp"
#include "System/ResourcesManager.hpp"
#include "GameObject.hpp"
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void InputManager(GLFWwindow* window, Camera& camera, float deltaTime, bool& lookAt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    camera.UpdateRotation({ (float)cursorX, (float)cursorY });

    bool sprint = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    float cameraSpeed = deltaTime * camera.Speed() + camera.Speed() * sprint * 0.2f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveTo({ 0.0f, 0.0f, -cameraSpeed });

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveTo({ 0.0f, 0.0f, cameraSpeed });

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveTo({ cameraSpeed, 0.0f, 0.0f });

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveTo({ -cameraSpeed, 0.0f, 0.0f });

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.MoveTo({ 0.0f, cameraSpeed, 0.0f });

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.MoveTo({ 0.0f, -cameraSpeed, 0.0f });

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lookAt = !lookAt;
}

int main()
{
    Render render(1400, 900);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(render.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader* shader = static_cast<Shader*>(ResourcesManager::AddResource<Shader>("shader", "resources/shader.vs", "resources/shader.fs"));
    Mesh* skullMesh = static_cast<Mesh*>(ResourcesManager::AddResource<Mesh>("Skull Mesh", "resources/Skull.obj"));
    Mesh* innMesh = static_cast<Mesh*>(ResourcesManager::AddResource<Mesh>("Inn Mesh", "resources/fantasy_game_inn.obj"));
    Texture* skullTexture = static_cast<Texture*>(ResourcesManager::AddResource<Texture>("Skull Texture", "resources/skull.jpg"));
    Texture* innTexture = static_cast<Texture*>(ResourcesManager::AddResource<Texture>("Inn Texture", "resources/fantasy_game_inn_diffuse.png"));

    Scene scene;

    GameObject* inn = GameObject::CreateGameObject();
    GameObject* skull = GameObject::CreateGameObject();
    inn->AddChild(skull);
    GameObject* skull2 = GameObject::CreateGameObject();
    skull2->SetParent(skull);

    inn->AddComponent<MeshRenderer>(innMesh, shader, innTexture);
    skull->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);
    skull2->AddComponent<MeshRenderer>(skullMesh, shader, skullTexture);
    
    skull2->position.x = -5;

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    Camera camera(Vec2((float)startCursorX, (float)startCursorY), render.width, render.height);

    float deltaTime = 0.0f;
    float lastFrame = (float)glfwGetTime();

    bool lookAt = false;
    float translation = 0.0f;
    while (glfwWindowShouldClose(render.window) == false)
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        InputManager(render.window, camera, deltaTime, lookAt);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        translation -= deltaTime;

		skull->position = { -3.0f + sin(translation), 0.0f, cos(translation) };

        scene.Draw();

        glfwSwapBuffers(render.window);
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    render.Destroy();

	return EXIT_SUCCESS;
}