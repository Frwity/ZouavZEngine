#include "System/TimeManager.hpp"
#include <GLFW/glfw3.h>

float TimeManager::time = (float)(glfwGetTime());
float TimeManager::deltaTime = (float)(glfwGetTime());
float TimeManager::previousTime = 0.0f;
float TimeManager::scale = 1.0f;

void TimeManager::Update()
{
	time = (float)(glfwGetTime());
	deltaTime = time - previousTime;
	previousTime = time;
}