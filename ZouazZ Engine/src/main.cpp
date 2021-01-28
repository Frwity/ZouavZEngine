#include "Maths/Mat4.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Resource.hpp"
#include "Rendering/Render.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/MeshRenderer.hpp"
#include "Component/Transform.hpp"
#include "GameObject.hpp"
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static Camera* MainCamera;

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


    std::unique_ptr<Shader> shader = std::make_unique<Shader>("resources/shader.vs", "resources/shader.fs");
    std::unique_ptr<Mesh> skullMesh = std::make_unique<Mesh>("resources/Skull.obj");
    std::unique_ptr<Texture> skullTexture = std::make_unique<Texture>("resources/skull.jpg");
    std::unique_ptr<Mesh> innMesh = std::make_unique<Mesh>("resources/fantasy_game_inn.obj");
    std::unique_ptr<Texture> innTexture = std::make_unique<Texture>("resources/fantasy_game_inn_diffuse.png");

    GameObject skull;
    GameObject inn;
    
    skull.AddComponent<MeshRenderer>(skullMesh.get(), shader.get(), skullTexture.get());
    inn.AddComponent<MeshRenderer>(innMesh.get(), shader.get(), innTexture.get());

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    Camera camera(Vec2((float)startCursorX, (float)startCursorY), render.width, render.height);
    MainCamera = &camera;
    

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    bool lookAt = false;
    float translation = 0.0f;
    while (glfwWindowShouldClose(render.window) == false)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        InputManager(render.window, camera, deltaTime, lookAt);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        translation -= deltaTime;

        skull.GetComponent<MeshRenderer>()->Draw(Mat4::CreateTranslationMatrix({ -3.0f + sin(translation), 0.0f, cos(translation) }), camera);
        inn.GetComponent<MeshRenderer>()->Draw(Mat4::Identity(), camera);

        glfwSwapBuffers(render.window);
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    render.Destroy();

	return EXIT_SUCCESS;
}