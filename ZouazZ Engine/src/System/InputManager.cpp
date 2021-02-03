#include "System/InputManager.hpp"
#include <GLFW/glfw3.h>

GLFWwindow* InputManager::window = nullptr;

bool InputManager::GetState(E_KEYS _key)
{
	return glfwGetKey(window, static_cast<int>(_key)) == GLFW_PRESS;
}

void InputManager::SetWindow(GLFWwindow* _window)
{
	window = _window;
}