
#include "System/Engine.hpp"

/*void InputManager(GLFWwindow* window, Camera& camera, float deltaTime, bool& lookAt)
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
}*/

int main()
{
    /*Render render(1400, 900);

    /*IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(render.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");*/

   /* Shader shader("resources/shader.vs", "resources/shader.fs");
    shader.Use();
    glUniform1i(glGetUniformLocation(shader.id, "ourTexture"), 0);

    //Mesh mesh("resources/fanta/*sy_game_inn.obj");
    //Texture texture("resources*fantasy_game_inn_diffuse.png");

    Mesh mesh("resources/Skull.obj");
    Texture texture("resources/skull.jpg");


    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(render.window, (int)render.width / 2, (int)render.height / 2);

    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    Camera camera({ (float)startCursorX, (float)startCursorY });

    Matrix4 projection = Matrix4::CreatePerspectiveProjectionMatrix(render.width, render.height, 0.01, 1000, 45);

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    bool lookAt = false;

    while (glfwWindowShouldClose(render.window) == false)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        InputManager(render.window, camera, deltaTime, lookAt);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glActiveTexture(GL_TEXTURE0);
        texture.Use();

        shader.SetMatrix("view", lookAt ? camera.GetLookAtMatrix(Vec3::Zero()) : camera.GetMatrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("model", Matrix4::Identity());

        glBindVertexArray(mesh.GetID());
        glDrawElements(GL_TRIANGLES, mesh.GetNbElements(), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(render.window);
    }

    /*ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    render.Destroy();*/

    Engine engine;

    engine.Update();


	return EXIT_SUCCESS;
}