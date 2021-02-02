#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "System/ResourcesManager.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/MeshRenderer.hpp"
#include "Rendering/Light.hpp"
#include "System/Engine.hpp"

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


Engine::Engine()
{
	render.Init(1400, 900);


    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           
    //ImGui::StyleColorsDark();

    //ImGui_ImplGlfw_InitForOpenGL(render.window, true);
    //ImGui_ImplOpenGL3_Init("#version 330");

    //TEMP
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);
    camera = Camera(Vec2((float)startCursorX, (float)startCursorY), render.width, render.height);
    camera.SetMainCamera();
    Load();
}

Engine::~Engine()
{
    /*ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();*/
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

    skull2->position.x = -5;

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    scene.lights.push_back(light1->GetComponent<Light>());
    scene.lights.push_back(light2->GetComponent<Light>());
    scene.lights.push_back(light3->GetComponent<Light>());
}

void Engine::Update()
{
    float deltaTime = 0.0f;
    float lastFrame = (float)glfwGetTime();

    bool lookAt = false;
    float translation = 0.0f;

    while (!render.Stop())
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        InputManager(render.window, camera, deltaTime, lookAt);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        translation -= deltaTime;

        scene.GetWorld().GetChildren().at(1)->position = { -3.0f + sin(translation), 0.0f, cos(translation) };

        scene.Draw();

        glfwSwapBuffers(render.window);
    }
}