#include "System/TimeManager.hpp"
#include <GLFW/glfw3.h>

float TimeManager::time = static_cast<float>(glfwGetTime());
float TimeManager::deltaTime = static_cast<float>(glfwGetTime());
float TimeManager::previousTime = 0.0f;
float TimeManager::scale = 1.0f;

void TimeManager::Update()
{
	time = static_cast<float>(glfwGetTime());
	deltaTime = time - previousTime;
	previousTime = time;
}