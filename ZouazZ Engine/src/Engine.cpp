#include "System/Engine.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void InputManager(GLFWwindow* window, Camera& camera, float deltaTime, bool& lookAt)
{
    glfwPollEvents();

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

    //TEMP
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    camera = Camera({ (float)startCursorX, (float)startCursorY });
}

Engine::~Engine()
{
	render.Destroy();
}

void Engine::Update()
{
    //TODO use resource manager
    Shader shader("resources/shader.vs", "resources/shader.fs");
    shader.Use();
    glUniform1i(glGetUniformLocation(shader.id, "ourTexture"), 0);

    Mesh mesh("resources/Skull.obj");
    Texture texture("resources/skull.jpg");

    Matrix4 projection = Matrix4::CreatePerspectiveProjectionMatrix(render.width, render.height, 0.01, 1000, 45);

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    bool lookAt = false;

    while (!render.Stop())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        InputManager(render.window, camera, deltaTime, lookAt);

        render.Clear();
        
        //SCENE
        shader.Use();
        glActiveTexture(GL_TEXTURE0);
        texture.Use();

        shader.SetMatrix("view", lookAt ? camera.GetLookAtMatrix(Vec3::Zero()) : camera.GetMatrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("model", Matrix4::Identity());

        render.Draw(mesh);
        
        render.Update();
    }
}